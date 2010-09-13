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

#include "rsc/logging/ConsoleLogger.h"

using namespace std;
using namespace rsc;
using namespace rsc::logging;

TEST(ConsoleLoggerTest, testLog)
{

    vector<Logger::Level> loggerLevels;
    loggerLevels.push_back(Logger::ALL);
    loggerLevels.push_back(Logger::TRACE);
    loggerLevels.push_back(Logger::DEBUG);
    loggerLevels.push_back(Logger::INFO);
    loggerLevels.push_back(Logger::WARN);
    loggerLevels.push_back(Logger::ERROR);
    loggerLevels.push_back(Logger::FATAL);
    loggerLevels.push_back(Logger::OFF);

    vector<Logger::Level> testLevels;
    testLevels.push_back(Logger::TRACE);
    testLevels.push_back(Logger::DEBUG);
    testLevels.push_back(Logger::INFO);
    testLevels.push_back(Logger::WARN);
    testLevels.push_back(Logger::ERROR);
    testLevels.push_back(Logger::FATAL);

    for (vector<Logger::Level>::iterator loggerLevelIt = loggerLevels.begin(); loggerLevelIt
            != loggerLevels.end(); ++loggerLevelIt) {

        Logger::Level loggerLevel = *loggerLevelIt;

        cout << "LoggerLevel: " << loggerLevel << endl;
        ConsoleLogger logger("TestLogger", loggerLevel);

        for (vector<Logger::Level>::iterator testLevelIt = testLevels.begin(); testLevelIt
                    != testLevels.end(); ++testLevelIt) {
            stringstream msg;
            msg << "Testing level " << *testLevelIt;
            logger.log(*testLevelIt, msg.str());

            if (*testLevelIt <= loggerLevel) {
                EXPECT_TRUE(logger.isEnabledFor(*testLevelIt));
            } else {
                EXPECT_FALSE(logger.isEnabledFor(*testLevelIt));
            }

        }

    }

}
