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

using namespace std;

namespace rsc {
namespace subprocess {

   #define TA_FAILED 0
   #define TA_SUCCESS_CLEAN 1
   #define TA_SUCCESS_KILL 2
   #define TA_SUCCESS_16 3

   DWORD TerminateApp( DWORD dwPID, DWORD dwTimeout );

   typedef struct
   {
      DWORD   dwID;
      DWORD   dwThread;
   } TERMINFO;

   // Declare Callback Enum Functions.
   bool CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam );

   /*----------------------------------------------------------------
   DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )

   Purpose:
      Shut down a 32-Bit Process (or 16-bit process under Windows 95)

   Parameters:
      dwPID
         Process ID of the process to shut down.

      dwTimeout
         Wait time in milliseconds before shutting down the process.

   Return Value:
      TA_FAILED - If the shutdown failed.
      TA_SUCCESS_CLEAN - If the process was shutdown using WM_CLOSE.
      TA_SUCCESS_KILL - if the process was shut down with
         TerminateProcess().
      NOTE:  See header for these defines.
   ----------------------------------------------------------------*/ 
   DWORD TerminateApp( DWORD dwPID, DWORD dwTimeout )
   {

      // If we can't open the process with PROCESS_TERMINATE rights,
      // then we give up immediately.
      HANDLE hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,
         dwPID);

      if(hProc == NULL)
      {
         return TA_FAILED;
      }

      // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
      // matches your process's.
      EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID);

      // Wait on the handle. If it signals, great. If it times out,
      // then you kill it.
      DWORD   dwRet;
      if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0) {
         dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
      } else {
         dwRet= TA_SUCCESS_CLEAN;
      }

      CloseHandle(hProc);

      return dwRet;
   }

   bool CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
   {
   
      DWORD dwID;
      GetWindowThreadProcessId(hwnd, &dwID);

      if(dwID == (DWORD)lParam)
      {
         PostMessage(hwnd, WM_CLOSE, 0, 0);
      }

      return true;
      
   }


WindowsSubprocess::WindowsSubprocess(const string &command,
    const vector<string> &args) {

    STARTUPINFO startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);

    // Try to start the process
    BOOL result = ::CreateProcess(
        "C:\\Windows\\NOTEPAD.exe",
        NULL,
        NULL,
        NULL,
        false,
        NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &startupInfo,
        &processInformation
        );

    if(result == 0) {
        throw runtime_error("Could not create process");
    }

}

WindowsSubprocess::~WindowsSubprocess() {
    TerminateApp(processInformation.dwProcessId, 5000);
}

}
}
