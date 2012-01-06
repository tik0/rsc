/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of the
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

#include "rsc/threading/Future.h"

using namespace std;
using namespace rsc::threading;
using namespace testing;

TEST(FutureTest, testIsDone)
{

    {
        Future<int> f;
        EXPECT_FALSE(f.isDone());
        f.setError("damn errors");
        EXPECT_TRUE(f.isDone());
    }

    {
        Future<int> f;
        EXPECT_FALSE(f.isDone());
        f.set(42);
        EXPECT_TRUE(f.isDone());
    }

}

TEST(FutureTest, testGetSingleThreaded)
{
    const int result = 10;
    {
        Future<int> f;
        f.set(result);
        EXPECT_EQ(result, f.get());
        EXPECT_EQ(result, f.get()) << "Repeated calls to get must be possible.";
    }
    {
        Future<int> f;
        f.set(result);
        EXPECT_EQ(result, f.get(1.0));
        EXPECT_EQ(result, f.get(1.0)) << "Repeated calls to get must be possible.";
    }
}

TEST(FutureTest, testGetException)
{

    {
        Future<int> f;
        f.setError("damn");
        EXPECT_THROW(f.get(), FutureTaskExecutionException); // without timeout first
        EXPECT_THROW(f.get(1.0), FutureTaskExecutionException);
    }

    {
        Future<int> f;
        f.setError("damn");
        EXPECT_THROW(f.get(1.0), FutureTaskExecutionException); // with timeout first
        EXPECT_THROW(f.get(), FutureTaskExecutionException);
    }
}

void setter(const int& result, Future<int>& future) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(300));
    future.set(result);
}

TEST(FutureTest, testGetAsynchronous)
{

    Future<int> f;
    const int result = 10;

    boost::thread t(boost::bind(&setter, boost::ref(result), boost::ref(f)));

    EXPECT_EQ(result, f.get());
    EXPECT_EQ(result, f.get(1.0));
}

TEST(FutureTest, testTimeout)
{
    Future<int> f;
    EXPECT_THROW(f.get(0.3), FutureTimeoutException);
}
