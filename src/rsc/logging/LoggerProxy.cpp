/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "LoggerProxy.h"

namespace rsc {
namespace logging {

LoggerProxy::SetLevelCallback::~SetLevelCallback() {
}

LoggerProxy::LoggerProxy(LoggerPtr logger, SetLevelCallbackPtr callback) :
        logger(logger), callback(callback) {
}

LoggerProxy::~LoggerProxy() {
}

Logger::Level LoggerProxy::getLevel() const {
    return logger->getLevel();
}

void LoggerProxy::setLevel(const Logger::Level& level) {
    //logger->setLevel(level);
    callback->call(shared_from_this(), level);
}

std::string LoggerProxy::getName() const {
    return logger->getName();
}

void LoggerProxy::setName(const std::string& name) {
    logger->setName(name);
}

void LoggerProxy::log(const Logger::Level& level, const std::string& msg) {
    logger->log(level, msg);
}

LoggerPtr LoggerProxy::getLogger() const {
    return logger;
}

void LoggerProxy::setLogger(LoggerPtr logger) {
    this->logger = logger;
}

}
}
