/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <string>
#include <vector>
#include <iostream>

#include <boost/static_assert.hpp>

#include <gtest/gtest.h>

#include <rsc/debug/DebugTools.h>

using namespace std;
using namespace rsc::debug;

TEST(DebugToolsTest, testNewInstance)
{
    EXPECT_TRUE(DebugTools::newInstance());
}

TEST(DebugToolsTest, testCreateBacktrace)
{
    vector<string> trace = DebugTools::newInstance()->createBacktrace();
    for (vector<string>::const_iterator it = trace.begin(); it != trace.end(); ++it) {
        cout << *it << endl;
    }
}
