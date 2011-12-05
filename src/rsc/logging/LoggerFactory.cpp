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

#include "LoggerFactory.h"

#include "ConsoleLoggingSystem.h"
#include "LoggerProxy.h"

using namespace std;
using namespace rsc::misc;

namespace rsc {
namespace logging {

const string LoggerFactory::DEFAULT_LOGGING_SYSTEM =
        ConsoleLoggingSystem::getLoggerName();

LoggerFactory::LoggerFactory() :
    currentLevel(Logger::LEVEL_WARN) {
    reselectLoggingSystem();
}

LoggerFactory::~LoggerFactory() {
}

void LoggerFactory::reselectLoggingSystem(const std::string& nameHint) {

    set<string> keys = loggingSystemRegistry()->getKnownRegistryKeys();
    assert(keys.count(DEFAULT_LOGGING_SYSTEM));

    string systemName;
    if (nameHint == DEFAULT_LOGGING_SYSTEM) {
        // use default if explicitly requested

        systemName = DEFAULT_LOGGING_SYSTEM;

    } else if (!nameHint.empty() && keys.count(nameHint)) {
        // use hint if available

        systemName = nameHint;

    } else {
        // auto-select fallback

        if (keys.size() > 1) {
            // do not use default if other options are available
            keys.erase(DEFAULT_LOGGING_SYSTEM);
        }
        systemName = *(keys.begin());

    }

    boost::recursive_mutex::scoped_lock lock(mutex);

	loggingSystem = loggingSystemRegistry()->getRegistree(systemName);

	// TODO update existing loggers to use the new logging system

}

LoggerPtr LoggerFactory::getLogger(const std::string& name) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    if (loggersByName.count(name)) {
        return loggersByName[name];
    } else {
        LoggerPtr logger(loggingSystem->createLogger(name));
        logger->setLevel(currentLevel);
        LoggerProxyPtr proxy(new LoggerProxy(logger));
        loggersByName[name] = proxy;
        return proxy;
    }
}

void LoggerFactory::reconfigure(const Logger::Level& level) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    currentLevel = level;
    for (map<string, LoggerProxyPtr>::iterator it = loggersByName.begin(); it
            != loggersByName.end(); ++it) {
        it->second->setLevel(level);
    }
}

string LoggerFactory::getLoggingSystemName() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return loggingSystem->getRegistryKey();
}

void LoggerFactory::clearKnownLoggers() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    loggersByName.clear();
}

}
}
