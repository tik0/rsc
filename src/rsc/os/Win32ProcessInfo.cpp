/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2014 Jan Moringen
 *
 * This file may be licensed under the terms of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
 *
 * ============================================================ */

#include "ProcessInfo.h"

#include <windows.h>
#include <Lmcons.h> // for UNLEN
#include <Psapi.h>

#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/shared_array.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/filetime_functions.hpp>

#include "Win32Common.h"

namespace rsc {
namespace os {

PID currentProcessId() {
    return GetCurrentProcessId(); // Cannot fail according to MSDN
}

std::string getProgramName(PID pid) {
    return getExecutablePath(pid);
}

std::string currentProgramName() {
    return currentExecutablePath();
}

std::string getExecutablePath(PID pid) {
    // will usually only work with elevated privileges

    HANDLE processHandle = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (processHandle == NULL) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                " open process with PID"
                " %1%: %2%") % pid % GetLastErrorString()));
    }

    TCHAR filename[MAX_PATH];
    DWORD ret = GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH);
    CloseHandle(processHandle);
    if (ret == 0) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                " get executable"
                " path: %1%") % GetLastErrorString()));
    }

    return std::string(filename);
}

std::string currentExecutablePath() {
    char buffer[MAX_PATH];
    int length;
    if ((length = GetModuleFileName(NULL, buffer, MAX_PATH)) == 0) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                " determine program"
                " name: %1%") % GetLastErrorString()));
    }
    return std::string(buffer, length);
}

typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
    HANDLE ProcessHandle,
    DWORD ProcessInformationClass,
    PVOID ProcessInformation,
    DWORD ProcessInformationLength,
    PDWORD ReturnLength
    );

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _PROCESS_BASIC_INFORMATION {
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

PVOID GetPebAddress(boost::shared_ptr<void> processHandle) {
    _NtQueryInformationProcess NtQueryInformationProcess =
            (_NtQueryInformationProcess) GetProcAddress(
                    GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
    PROCESS_BASIC_INFORMATION pbi;
    NtQueryInformationProcess(processHandle.get(), 0, &pbi, sizeof(pbi), NULL);

    return pbi.PebBaseAddress;
}

class DeleteHandle {
public:

    void operator()(void* handle) {
        if (handle != 0) {
            CloseHandle((HANDLE) handle);
        }
    }

};

std::vector<std::string> getCommandlineArguments(PID pid) {
    // Will only work with elevated privileges for other processes
    // than the current.
    //
    // Adapted from:
    // http://wj32.org/wp/2009/01/24/howto-get-the-command-line-of-processes/
    //
    // Also note that this is not reliable:
    // http://blogs.msdn.com/b/oldnewthing/archive/2009/11/25/9928372.aspx

    HANDLE rawProcessHandle = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (rawProcessHandle == 0) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                " open process with PID"
                " %1%: %2%") % pid % GetLastErrorString()));
    }
    // Maintain the process handle in a shared_ptr so that it can be
    // correctly closed even in case of exceptions.
    boost::shared_ptr<void> processHandle(rawProcessHandle, DeleteHandle());

    PVOID pebAddress = GetPebAddress(processHandle);

    // get the address of ProcessParameters
    PVOID rtlUserProcParamsAddress = 0;
    if (!ReadProcessMemory(processHandle.get(), (PCHAR) pebAddress + 0x10,
            &rtlUserProcParamsAddress, sizeof(PVOID), NULL)) {
        throw std::runtime_error(
                boost::str(
                        boost::format(
                                "Could not read the address of ProcessParameters: %1%")
                                % GetLastErrorString()));
    }

    // read the CommandLine UNICODE_STRING structure
    UNICODE_STRING commandLine;
    if (!ReadProcessMemory(processHandle.get(),
            (PCHAR) rtlUserProcParamsAddress + 0x40, &commandLine,
            sizeof(commandLine), NULL)) {
        throw std::runtime_error(
                boost::str(
                        boost::format("Could not read CommandLine: %1%")
                                % GetLastErrorString()));
    }

    // allocate memory to hold the command line
    boost::shared_array<WCHAR> commandLineContents(
            new WCHAR[commandLine.Length]);

    // read the command line
    if (!ReadProcessMemory(processHandle.get(), commandLine.Buffer,
            commandLineContents.get(), commandLine.Length, NULL)) {
        throw std::runtime_error(
                boost::str(
                        boost::format(
                                "Could not read the command line string: %1%")
                                % GetLastErrorString()));
    }

    std::wstring wideCommandLineString = commandLineContents.get();
    // The conversion from wstring to string relies on truncating the
    // multibytes while assuming that for common characters, the first
    // byte matches ASCII: http://stackoverflow.com/a/12097772/283649
    //
    // Moreover, it seems there is no proper null termination here, so
    // calculate the end manually.
    std::string commandLineString(wideCommandLineString.begin(),
            wideCommandLineString.begin() + commandLine.Length / 2);

    // tokenize the command line
    std::vector<std::string> arguments;
    boost::split(arguments, commandLineString, boost::is_any_of(" "),
            boost::token_compress_on);

    return arguments;
}

// Based on MSDN example for CommandLineToArgv
std::vector<std::string> currentCommandlineArguments() {
    LPWSTR* arguments;
    int numArguments;
    if ((arguments = CommandLineToArgvW(GetCommandLineW(), &numArguments))
         == NULL) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " commandline"
                                                          " arguments: %1%")
                                            % GetLastErrorString()));
    }

    std::vector<std::string> result;
    for (int i = 0; i < numArguments; ++i) {
        std::wstring arg = arguments[i];
        // The conversion from wstring to string relies on truncating
        // the multibytes while assuming that for common characters,
        // the first byte matches ASCII:
        // http://stackoverflow.com/a/12097772/283649
        result.push_back(std::string(arg.begin(), arg.end()));
    }
    LocalFree(arguments);

    return result;
}

boost::posix_time::ptime getProcessStartTime(PID pid) {
    FILETIME creationTime;
    FILETIME exitTime;     // not used
    FILETIME kernelTime;   // not used
    FILETIME userTime;     // not used

    HANDLE processHandle = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (processHandle == NULL) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                " open process with PID"
                " %1%: %2%") % pid % GetLastErrorString()));
    }

    BOOL ret = GetProcessTimes(processHandle,
                        &creationTime, &exitTime, &kernelTime, &userTime);
    CloseHandle(processHandle);
    if (ret ==0) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                                                          " determine process"
                                                          " start time: %1%")
                                            % GetLastErrorString()));
    }

    return boost::date_time::time_from_ftime<boost::posix_time::ptime>(creationTime);
}

boost::posix_time::ptime currentProcessStartTime() {
    return getProcessStartTime(currentProcessId());
}

std::string getExecutingUser(PID /*pid*/) {
    throw std::runtime_error("Could not determine executing user:"
                             " not supported");
}

std::string currentExecutingUser() {
    TCHAR buffer[UNLEN + 1];
    DWORD size = UNLEN + 1;
    if (GetUserName((TCHAR*) buffer, &size)) {
        std::wstring name((wchar_t*) buffer, size - 1);
        return std::string(name.begin(), name.end());
    } else {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " executing user: %1%")
                                            % GetLastErrorString()));
    }
}

}
}
