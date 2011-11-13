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

#include <map>

#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "Logger.h"
#include "LoggingSystem.h"
#include "../misc/Registry.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace logging {

/**
 * Factory to create logger instances. On singleton creation selects a logger
 * according to the heuristic mentioned for #reselectLoggingSystem.
 *
 * @author jwienke
 */
class RSC_EXPORT LoggerFactory: boost::noncopyable {
private:
    LoggerFactory();

public:
    virtual ~LoggerFactory();

    /**
     * Returns the singleton instance of this class.
     *
     * @todo is this method thread-safe? Singleton in C++ and static check needed
     * @return single instance
     */
    static LoggerFactory* getInstance();

    /**
     * Get a logger for the given name. If a logger with this name already
     * exists, the existing instance is returned.
     *
     * @todo think about logger hierarchy
     * @param name name of the logger
     * @return logger instance
     */
    LoggerPtr getLogger(const std::string& name);

    /**
     * Simple hack to reconfigure all known loggers and new instances with a
     * logging level.
     *
     * @todo better configuration system required
     * @param level new level for all loggers
     */
    void reconfigure(const Logger::Level& level);

    /**
     * Reselected the automatically chosen logging system to adapt to newly
     * available ones. A hint can be given on the name of the logging system to
     * select.
     *
     * If no hint is given the first found logging system which is not the
     * default will be selected. If no such system exists, the default is used.
     * The name hint overrides these settings if a logging system matching the
     * hint is found.
     *
     * This method will NOT change previously gathered loggers.
     *
     * @param nameHint hint for the name of the logging system to select. If the
     *                 string is empty, it will not be used. If it is
     *                 #DEFAULT_LOGGING_SYSTEM, the default logging system will
     *                 be selected even if other systems are available.
     * @todo create a decorator for Logger and return this in the smart pointers
     *       to enable changing the logger instance at runtime
     */
    void reselectLoggingSystem(const std::string& nameHint = "");

    /**
     * Parameter to pass as name hint to #reselectLoggingSystem for selecting
     * the default system.
     */
    static const std::string DEFAULT_LOGGING_SYSTEM;

    /**
     * Returns the name of the currently selected logging system.
     *
     * @return logging system name
     */
    std::string getLoggingSystemName();

    /**
     * Do not use this, for testing only!
     */
    void clearKnownLoggers();

private:

    boost::shared_ptr<LoggingSystem> loggingSystem;

    boost::recursive_mutex mutex;
    std::map<std::string, LoggerPtr> loggersByName;

    Logger::Level currentLevel;

};

}
}

