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

#ifndef WINDOWSSUBPROCESS_H_
#define WINDOWSSUBPROCESS_H_

#include <string>
#include <vector>

#include <windows.h>

#include "Subprocess.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace subprocess {

/**
 * Subprocess implementation for windows.
 *
 * @author jwienke
 */
class RSC_EXPORT WindowsSubprocess: public Subprocess {
public:

    WindowsSubprocess(const std::string &command,
            const std::vector<std::string> &args = std::vector<std::string>());
    virtual ~WindowsSubprocess();

private:

    enum TerminateResult {
        FAILED, SUCCESS_CLEAN, SUCCESS_KILL
    };

    typedef struct {
        DWORD id;
        DWORD thread;
    } TermInfo;

    TerminateResult terminateApp(DWORD pid, DWORD timeoutMs);

    PROCESS_INFORMATION processInformation;

};

}
}

#endif /* WINDOWSSUBPROCESS_H_ */
