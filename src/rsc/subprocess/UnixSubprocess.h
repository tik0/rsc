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

#pragma once

#include "Subprocess.h"

#include <sys/types.h>
#include <unistd.h>

#include "../logging/Logger.h"

namespace rsc {
namespace subprocess {

/**
 * Unix subprocess implementation.
 *
 * @author jwienke
 * @todo somehow this crashes and does not give an exception if you try to
 *       start a binary that does not exist
 */
class UnixSubprocess: public Subprocess {
public:
    UnixSubprocess(const std::string &command,
            const std::vector<std::string> &args);
    virtual ~UnixSubprocess();

private:

    logging::LoggerPtr logger;

    std::string command;

    pid_t pid;

    char **args;
    size_t argLen;

};

}
}

