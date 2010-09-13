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

#ifndef MOCKS_H_
#define MOCKS_H_

#include "rsc/logging/Logger.h"
#include "rsc/logging/LoggingSystem.h"

class MockLogger: public rsc::logging::Logger {
public:
MOCK_CONST_METHOD0(getLevel, rsc::logging::Logger::Level());
MOCK_METHOD1(setLevel, void(const Level &level));
MOCK_CONST_METHOD0(getName, std::string());
MOCK_METHOD1(setName, void(const std::string &name));
MOCK_METHOD2(log, void(const rsc::logging::Logger::Level &level, const std::string &msg));
};

class MockLoggingSystem: public rsc::logging::LoggingSystem {
public:
    std::string name;
    MockLoggingSystem(const std::string &name) :
        name(name) {
    }
    std::string getRegistryKey() const {
        return name;
    }
MOCK_METHOD1(createLogger, rsc::logging::LoggerPtr(const std::string &name));
};

#endif /* MOCKS_H_ */
