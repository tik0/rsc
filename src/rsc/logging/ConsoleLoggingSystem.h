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

#ifndef CONSOLELOGGINGSYSTEM_H_
#define CONSOLELOGGINGSYSTEM_H_

#include "LoggingSystem.h"

namespace rsc {
namespace logging {

/**
 * Default logging system using the console for output.
 *
 * @author jwienke
 */
class ConsoleLoggingSystem: public LoggingSystem {
public:

    ConsoleLoggingSystem();
    virtual ~ConsoleLoggingSystem();

    std::string getRegistryKey() const;

    LoggerPtr createLogger(const std::string &name);

    static std::string getLoggerName();

};

}
}

#endif /* CONSOLELOGGINGSYSTEM_H_ */
