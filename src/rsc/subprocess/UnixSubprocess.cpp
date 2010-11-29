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

#include "UnixSubprocess.h"

#include <stdexcept>
#include <sstream>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

namespace rsc {
namespace subprocess {

UnixSubprocess::UnixSubprocess(const string &command, const vector<string> args) :
    logger(logging::Logger::getLogger("rsc.subprocess.UnixSubprocess")),
            argLen(args.size() + 2) {

    if (logger->isDebugEnabled()) {
        stringstream argStream;
        argStream << "[";
        for (size_t i = 0; i < args.size(); ++i) {
            argStream << args[i];
            if (i != args.size() - 1) {
                argStream << ", ";
            } else {
                argStream << "]";
            }
        }
        RSCDEBUG(logger, "Creating a subprocess for command '" << command
                << "' with arguments " << argStream.str());
    }

    // convert arguments to char pointer array
    this->args = new char *[argLen];
    this->args[0] = new char[strlen(command.c_str()) + 1];
    strcpy(this->args[0], command.c_str());
    for (size_t i = 0; i < args.size(); ++i) {
        this->args[i + 1] = new char[strlen(args[i].c_str()) + 1];
        strcpy(this->args[i + 1], args[i].c_str());
    }
    this->args[args.size() + 1] = 0;

    pid_t pid = vfork();
    if (pid == 0) {

        // child process
        int ret = execv(command.c_str(), this->args);
        if (ret == -1) {
            RSCERROR(logger, "Error starting subprocess command '" << command
                    << "': " << strerror(errno));
        }

    } else if (pid < 0) {

        // failed to fork
        throw runtime_error("Forking new subprocess failed.");

    } else {

        // code executed by parent process
        this->pid = pid;

    }

}

UnixSubprocess::~UnixSubprocess() {

    kill(pid, SIGINT);
    int status;
    pid_t terminated;
    while (!(terminated = waitpid(pid, &status, WNOHANG))) {
        // TODO introduce a timeout
        continue;
    }

    // clean up argument structure
    for (size_t i = 0; i < argLen - 1; ++i) {
        delete[] args[i];
    }
    delete[] args;

}

}
}
