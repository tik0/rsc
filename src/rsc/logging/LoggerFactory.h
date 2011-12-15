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
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "Logger.h"
#include "LoggerTreeNode.h"
#include "LoggerProxy.h"
#include "LoggingSystem.h"
#include "../misc/Registry.h"
#include "../patterns/Singleton.h"
#include "../misc/langutils.h"
#include "rsc/rscexports.h"

namespace rsc {

/**
 * Provides a hierarchical logging system with the possibility to install
 * different backends, which are instances of LoggingSystem. The hierarchy is
 * completely maintained by LoggerFactory, hence logging systems only need to
 * provide Logger instances which are configured externally according to the
 * hierarchy.
 *
 * Conceptually, the hierarchy is based on log4cxx/log4j. We have a root logger
 * with empty string as name. All other loggers are (indirect) children of this
 * logger. Hierarchies are separated by a '.' in the logger name. At startup
 * only the root logger has an assigned level. All other loggers inherit this
 * level if not one of their parents also has a level assigned. Generally,
 * the effective level of a logger is the assigned level of the earliest parent
 * of the logger which actually has an assigned level.
 *
 * To install new LoggingSystem instances, register them in #loggingSystemRegistry.
 * The selection of a logging system can be triggered through LoggerFactory#reselectLoggingSystem using a string as a hint.
 *
 * As the default, a simple cout- / cerr-based LoggingSystem called
 * ConsoleLoggingSystem is provided.
 */
namespace logging {

/**
 * Factory to create logger instances. On singleton creation selects a logger
 * according to the heuristic mentioned for #reselectLoggingSystem.
 *
 * @author jwienke
 */
class RSC_EXPORT LoggerFactory: public patterns::Singleton<LoggerFactory> {
private:
    LoggerFactory();

public:

    friend class patterns::Singleton<LoggerFactory>;

    virtual ~LoggerFactory();

    /**
     * Get a logger for the given name. If a logger with this name already
     * exists, the existing instance is returned.
     *
     * @param name name of the logger, empty string means root logger
     * @return logger instance
     */
    LoggerPtr getLogger(const std::string& name = "");

    /**
     * Simple hack to reconfigure all known loggers and new instances with a
     * logging level. Only loggers which had a manually assigned level will
     * actually be assigned with the new level. All others use the inheritance
     * tree to get a level. Effectively this will result in all loggers
     * appearing with the same level but semantics for changing certain levels
     * in the logger tree are preserved.
     *
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
     */
    void reselectLoggingSystem(const std::string& nameHint = "");

    /**
     * Parameter to pass as name hint to #reselectLoggingSystem for selecting
     * the default system.
     */
    static const std::string DEFAULT_LOGGING_SYSTEM;

    /**
     * Default level when the system is used without prior initialization.
     */
    static const Logger::Level DEFAULT_LEVEL;

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

    class ReconfigurationVisitor;
    class ReselectVisitor;

    LoggerProxyPtr createLogger(const LoggerTreeNode::NamePath& path,
            LoggerTreeNodePtr node);

    boost::shared_ptr<LoggingSystem> loggingSystem;

    boost::recursive_mutex mutex;
    LoggerTreeNodePtr loggerTree;

};

}
}

