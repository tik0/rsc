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

#include "Logger.h"

#include "LoggerFactory.h"

using namespace std;

namespace rsc {
namespace logging {

Logger::~Logger() {
}

LoggerPtr Logger::getLogger(const std::string &name) {
    return LoggerFactory::getInstance()->getLogger(name);
}

void Logger::trace(const string &msg) {
    this->log(LEVEL_TRACE, msg);
}

void Logger::debug(const string &msg) {
    this->log(LEVEL_DEBUG, msg);
}

void Logger::info(const string &msg) {
    this->log(LEVEL_INFO, msg);
}

void Logger::warn(const string &msg) {
    this->log(LEVEL_WARN, msg);
}

void Logger::error(const string &msg) {
    this->log(LEVEL_ERROR, msg);
}

void Logger::fatal(const string &msg) {
    this->log(LEVEL_FATAL, msg);
}

bool Logger::isTraceEnabled() const {
    return isEnabledFor(LEVEL_TRACE);
}

bool Logger::isDebugEnabled() const {
    return isEnabledFor(LEVEL_DEBUG);
}

bool Logger::isInfoEnabled() const {
    return isEnabledFor(LEVEL_INFO);
}

bool Logger::isWarnEnabled() const {
    return isEnabledFor(LEVEL_WARN);
}

bool Logger::isErrorEnabled() const {
    return isEnabledFor(LEVEL_ERROR);
}

bool Logger::isFatalEnabled() const {
    return isEnabledFor(LEVEL_FATAL);
}

bool Logger::isEnabledFor(const Level &level) const {
    return level <= getLevel();
}

ostream &operator<<(ostream &stream, const Logger::Level &level) {
    switch (level) {
    case Logger::LEVEL_ALL:
        stream << "ALL";
        break;
    case Logger::LEVEL_TRACE:
        stream << "TRACE";
        break;
    case Logger::LEVEL_DEBUG:
        stream << "DEBUG";
        break;
    case Logger::LEVEL_INFO:
        stream << "INFO";
        break;
    case Logger::LEVEL_WARN:
        stream << "WARN";
        break;
    case Logger::LEVEL_ERROR:
        stream << "ERROR";
        break;
    case Logger::LEVEL_FATAL:
        stream << "FATAL";
        break;
    case Logger::LEVEL_OFF:
        stream << "OFF";
        break;
    default:
        assert(false);
        stream << "UNKNOWN-LEVEL";
    }
    return stream;
}

}
}
