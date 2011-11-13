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

#include <fstream>

#include <boost/format.hpp>

#include <gtest/gtest.h>

#include "rsc/config/Environment.h"

using namespace std;
using namespace boost;
using namespace rsc::config;

TEST(EnvironmentTest, testUserHomeDirectory)
{
    try {
        userHomeDirectory();
    } catch (runtime_error& e) {
        // may happen if windows service
    }
}

TEST(EnvironmentTest, testUserConfigDirectory)
{
    try {
        userConfigDirectory();
    } catch (runtime_error& e) {
        // may happen if windows service
    }
}

