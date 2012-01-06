/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
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
