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

    Future<int> f;
    const int result = 10;
    f.set(result);
    EXPECT_EQ(result, f.get());
    EXPECT_EQ(result, f.get()) << "Repeated calls to get must be possible.";

}

void setter(const int &result, Future<int> &future) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(300));
    future.set(result);
}

TEST(FutureTest, testGetAsynchronous)
{

    Future<int> f;
    const int result = 10;

    boost::thread t(boost::bind(&setter, boost::ref(result), boost::ref(f)));

    EXPECT_EQ(result, f.get());

}
