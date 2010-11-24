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

#ifndef LOGGINGSYSTEM_H_
#define LOGGINGSYSTEM_H_

#include <string>

#include "Logger.h"
#include "../misc/Registry.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace logging {

/**
 * Represents a logging adapter available to the rsc logging system through the
 * configuration. Reentrancy is required.
 *
 * @author jwienke
 */
class RSC_EXPORT LoggingSystem {
public:

    /**
     * Returns a unique key describing the name of the logging system.
     *
     * @return name of the logging system
     */
    virtual std::string getRegistryKey() const = 0;

    /**
     * Factory method to create a new system-specific logger. Given parameters
     * to this method are assumed to be set on the returned logger. The caller
     * is responsible to not request a logger with the same name multiple times.
     * Otherwise it is legal to return a second instance in this method.
     *
     * @param name name of the new logger
     * @return new logger instance
     */
    virtual LoggerPtr createLogger(const std::string &name) = 0;

};

RSC_EXPORT CREATE_REGISTRY(LoggingSystem, loggingSystemRegistry);

}
}

#endif /* LOGGINGSYSTEM_H_ */
