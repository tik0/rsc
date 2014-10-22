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

#include <stdexcept>

#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/filetime_functions.hpp>

#include "Win32Common.h"

namespace rsc {
namespace os {

PID currentProcessId() {
    return GetCurrentProcessId(); // Cannot fail according to MSDN
}

std::string getProgramName(PID /*pid*/) {
    throw std::runtime_error("Could not determine program name: not supported");
}

std::string currentProgramName() {
    char buffer[MAX_PATH];
    int length;
    if ((length = GetModuleFileName(NULL, buffer, MAX_PATH)) == 0) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                " determine program"
                " name: %1%") % GetLastErrorString()));
    }
    return std::string(buffer, length);
}

std::string getExecutablePath(PID /*pid*/) {
    throw std::runtime_error("Could not determine executable path: not supported");
}

std::string currentExecutablePath() {
    throw std::runtime_error("Could not determine executable path: not supported");
}

std::vector<std::string> getCommandlineArguments(PID /*pid*/) {
    throw std::runtime_error("Could not determine commandline arguments: not supported");
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
		// the conversion from wstring to string relies on truncating the
		// multibytes while assuming that for common characters, the first
		// byte matches ASCII:
		// http://stackoverflow.com/a/12097772/283649
        result.push_back(std::string(arg.begin(), arg.end()));
    }
    LocalFree(arguments);

    return result;
}

boost::posix_time::ptime getProcessStartTime(PID /*pid*/) {
    throw std::runtime_error("Could not determine process start time: not supported");
}

boost::posix_time::ptime currentProcessStartTime() {
    FILETIME creationTime;
    FILETIME exitTime;     // not used
    FILETIME kernelTime;   // not used
    FILETIME userTime;     // not used

    if (GetProcessTimes(GetCurrentProcess(),
                        &creationTime, &exitTime, &kernelTime, &userTime)
        == 0) {
        throw std::runtime_error(boost::str(boost::format("Could not"
                                                          " determine process"
                                                          " start time: %1%")
                                            % GetLastError()));
    }

    return boost::date_time::time_from_ftime<boost::posix_time::ptime>(creationTime);
}

}
}
