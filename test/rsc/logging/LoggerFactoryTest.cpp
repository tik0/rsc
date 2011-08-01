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

#include "rsc/logging/ConsoleLoggingSystem.h"
#include "rsc/logging/LoggerFactory.h"
#include "rsc/misc/langutils.h"

#include "mocks.h"

using namespace std;
using namespace rsc;
using namespace rsc::logging;
using namespace rsc::misc;
using namespace testing;

TEST(LoggerFactoryTest, testSingleton)
{
    EXPECT_EQ(LoggerFactory::getInstance(), LoggerFactory::getInstance());
}

TEST(LoggerFactoryTest, testSingletonLoggers)
{
    LoggerFactory::getInstance()->reconfigure(Logger::WARN);
    stringstream name;
    name << rand();
    LoggerPtr logger = LoggerFactory::getInstance()->getLogger(name.str());
    EXPECT_EQ(Logger::WARN, logger->getLevel());
    EXPECT_EQ(logger, LoggerFactory::getInstance()->getLogger(name.str()));
    EXPECT_NE(logger, LoggerFactory::getInstance()->getLogger(name.str() + "blubb"));
}

TEST(LoggerFactoryTest, testReconfigure)
{
    LoggerFactory::getInstance()->reconfigure(Logger::ERROR);
    stringstream name;
    name << rand();
    LoggerPtr logger = LoggerFactory::getInstance()->getLogger(name.str());
    EXPECT_EQ(Logger::ERROR, logger->getLevel());

    LoggerFactory::getInstance()->reconfigure(Logger::TRACE);
    EXPECT_EQ(Logger::TRACE, logger->getLevel());
    stringstream name2;
    name2 << rand();
    LoggerPtr logger2 = LoggerFactory::getInstance()->getLogger(name2.str());
    EXPECT_EQ(Logger::TRACE, logger2->getLevel());
}

TEST(LoggerFactoryTest, testReselectLoggingSystem)
{

    // ensure that there are other logging systems
    StrictMock<MockLoggingSystem> *l1 = new StrictMock<MockLoggingSystem> (
            randAlnumStr(25));
    loggingSystemRegistry()->addRegistree(l1);
    StrictMock<MockLoggingSystem> *l2 = new StrictMock<MockLoggingSystem> (
            randAlnumStr(25));
    loggingSystemRegistry()->addRegistree(l2);

    // force default
    LoggerFactory::getInstance()->reselectLoggingSystem(
            LoggerFactory::DEFAULT_LOGGING_SYSTEM);
    EXPECT_EQ(ConsoleLoggingSystem::getLoggerName(), LoggerFactory::getInstance()->getLoggingSystemName());
    // if the logging system is illegally reported by the line above the next
    // line will cause the mock object to report an error
    LoggerPtr logger(LoggerFactory::getInstance()->getLogger(randAlnumStr(130)));

    // force hint
    string name = randAlnumStr(10);
    EXPECT_CALL(*l2, createLogger(name)).Times(1).WillOnce(Return(LoggerPtr(new NiceMock<MockLogger>)));
    LoggerFactory::getInstance()->reselectLoggingSystem(
            l2->name);
    EXPECT_EQ(l2->name, LoggerFactory::getInstance()->getLoggingSystemName());
    logger = LoggerFactory::getInstance()->getLogger(name);

    LoggerFactory::getInstance()->reselectLoggingSystem(
            LoggerFactory::DEFAULT_LOGGING_SYSTEM);
    loggingSystemRegistry()->removeRegistree(l1->name);

    // wrong hint, fallback to other available system
    string newName = randAlnumStr(12);
    EXPECT_CALL(*l2, createLogger(newName)).Times(1).WillOnce(Return(LoggerPtr(new NiceMock<MockLogger>)));
    LoggerFactory::getInstance()->reselectLoggingSystem(
            randAlnumStr(123));
    EXPECT_EQ(l2->name, LoggerFactory::getInstance()->getLoggingSystemName());
    logger = LoggerFactory::getInstance()->getLogger(newName);

    // cleanup mocks
    LoggerFactory::getInstance()->reselectLoggingSystem(
            LoggerFactory::DEFAULT_LOGGING_SYSTEM);
    loggingSystemRegistry()->removeRegistree(l2->name);

    LoggerFactory::getInstance()->clearKnownLoggers();

}
