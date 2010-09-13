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

#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/logging/Logger.h"
#include "rsc/misc/langutils.h"

#include "mocks.h"

using namespace std;
using namespace rsc;
using namespace rsc::logging;
using namespace testing;

TEST(LoggerFactoryTest, testLogMacros)
{

    string expect = "one 2 three";

    map<Logger::Level, unsigned int> levelCalls;
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::ALL, 0));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::TRACE, 0));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::DEBUG, 1));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::INFO, 2));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::WARN, 3));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::ERROR, 4));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::FATAL, 5));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::OFF, 6));

    vector<Logger::Level> expectedLevels;
    expectedLevels.push_back(Logger::TRACE);
    expectedLevels.push_back(Logger::DEBUG);
    expectedLevels.push_back(Logger::INFO);
    expectedLevels.push_back(Logger::WARN);
    expectedLevels.push_back(Logger::ERROR);
    expectedLevels.push_back(Logger::FATAL);

    for (map<Logger::Level, unsigned int>::iterator it = levelCalls.begin(); it
            != levelCalls.end(); ++it) {

        StrictMock<MockLogger> loggerMock;

        EXPECT_CALL(loggerMock, getLevel()).Times(AnyNumber()).WillRepeatedly(
                Return(it->first));
        for (unsigned int i = it->second; i < expectedLevels.size(); ++i) {
            EXPECT_CALL(loggerMock, log(expectedLevels[i], expect));
        }

        LoggerPtr logger(&loggerMock, rsc::misc::NullDeleter());

        RSCTRACE(logger, "one " << 2 << " three");
        RSCDEBUG(logger, "one " << 2 << " three");
        RSCINFO(logger, "one " << 2 << " three");
        RSCWARN(logger, "one " << 2 << " three");
        RSCERROR(logger, "one " << 2 << " three");
        RSCFATAL(logger, "one " << 2 << " three");

    }

}
