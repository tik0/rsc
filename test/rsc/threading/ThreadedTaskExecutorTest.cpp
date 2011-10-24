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

#include <iostream>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/threading/SimpleTask.h"
#include "rsc/threading/ThreadedTaskExecutor.h"

using namespace std;
using namespace rsc::threading;
using namespace rsc::misc;
using namespace testing;

class FlagSetTask: public SimpleTask {
public:
    boost::uint64_t runTime;
    FlagSetTask() : runTime(0) {
    }
    void run() {
        runTime = currentTimeMicros();
        markDone();
    }
};

class ThreadedTaskExecutorTest: public ::testing::Test {
public:

    void SetUp() {
        executor = ThreadedTaskExecutor();
    }

    ThreadedTaskExecutor executor;

};

TEST_F(ThreadedTaskExecutorTest, testScheduleExceptionCanceled) {

    TaskPtr t(new FlagSetTask);
    t->cancel();
    EXPECT_THROW(executor.schedule(t), invalid_argument);
    EXPECT_THROW(executor.schedule(t, 320), invalid_argument);

}

TEST_F(ThreadedTaskExecutorTest, testScheduleDelayedAfterCancel) {

    TaskPtr t(new FlagSetTask);
    EXPECT_FALSE(t->isDone());
    executor.schedule(t, 100000);
    t->cancel();
    EXPECT_FALSE(t->isDone());
    t->waitDone();
    EXPECT_TRUE(t->isDone());

}

TEST_F(ThreadedTaskExecutorTest, testScheduleDelayed) {

    const boost::uint64_t delay = 237048;
    boost::shared_ptr<FlagSetTask> t(new FlagSetTask);
    boost::uint64_t scheduleTime = currentTimeMicros();
    executor.schedule(t, delay);
    t->waitDone();
    const boost::uint64_t allowedPrecision = 100000;
    EXPECT_GE(t->runTime, scheduleTime + delay - allowedPrecision);
    EXPECT_LE(t->runTime, scheduleTime + delay + allowedPrecision);

}
