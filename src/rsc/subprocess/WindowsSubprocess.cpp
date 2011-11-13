/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "WindowsSubprocess.h"

#include <sstream>

using namespace std;

namespace rsc {
namespace subprocess {

bool CALLBACK terminateAppEnum(HWND hwnd, LPARAM lParam) {

    DWORD id;
    GetWindowThreadProcessId(hwnd, &id);

    if(id == (DWORD)lParam)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }

    return true;

}

WindowsSubprocess::TerminateResult WindowsSubprocess::terminateApp(DWORD pid,
        DWORD timeoutMs) {

    // If we can't open the process with PROCESS_TERMINATE rights,
    // then we give up immediately.
    HANDLE process = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, false, pid);

    if (process == NULL) {
        return FAILED;
    }

    // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
    // matches your process's.
    EnumWindows((WNDENUMPROC) terminateAppEnum, (LPARAM) pid);

    // Wait on the handle. If it signals, great. If it times out,
    // then you kill it.
    TerminateResult terminateReturn;
    if (WaitForSingleObject(process, timeoutMs) != WAIT_OBJECT_0) {
        terminateReturn
                = (TerminateProcess(process, 0) ? SUCCESS_KILL : FAILED);
    } else {
        terminateReturn = SUCCESS_CLEAN;
    }

    CloseHandle(process);

    return terminateReturn;
}

WindowsSubprocess::WindowsSubprocess(const string& command,
        const vector<string>& args) {

    STARTUPINFO startupInfo = { 0 };
    startupInfo.cb = sizeof(startupInfo);

    stringstream commandLine;
    commandLine << command << " ";
    for (vector<string>::const_iterator argIt = args.begin(); argIt
            != args.end(); ++argIt) {
        commandLine << "\"" <<  *argIt << "\" ";
    }

    // Try to start the process
    bool result = ::CreateProcess(NULL,
            const_cast<char*> (commandLine.str().c_str()), NULL, NULL, false,
            NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo,
            &processInformation);

    if (result == 0) {
        throw runtime_error("Could not create process");
    }

}

WindowsSubprocess::~WindowsSubprocess() {
    terminateApp(processInformation.dwProcessId, 5000);
}

}
}
