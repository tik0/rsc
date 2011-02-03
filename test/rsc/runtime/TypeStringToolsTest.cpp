/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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

#include <gtest/gtest.h>

#include <rsc/runtime/TypeStringTools.h>

using namespace std;
using namespace rsc::runtime;

TEST(TypeStringToolsTest, test_type_name)
{
    EXPECT_EQ(type_name(typeid(bool)), "bool");
    EXPECT_EQ(type_name(typeid(int)), "int");
    EXPECT_EQ(type_name(typeid(string)), "std::string");

    EXPECT_EQ(type_name<bool>(), "bool");
    EXPECT_EQ(type_name<int>(), "int");
    EXPECT_EQ(type_name<string>(), "std::string");

    EXPECT_EQ(type_name(true), "bool");
    EXPECT_EQ(type_name(1), "int");
    EXPECT_EQ(type_name(string("bla")), "std::string");
}

TEST(TypeStringToolsTest, test_type_string)
{
    EXPECT_EQ(type_string("known type %1%",
                    "unknown type",
                    true),
            "known type 1");
    EXPECT_EQ(type_string("known type %1%",
                    "unknown type",
                    5),
            "known type 5");
    EXPECT_EQ(type_string("known type %1%",
                    "unknown type",
                    string("test")),
            "known type test");
    EXPECT_EQ(type_string("known type %1%",
                    "unknown type",
                    vector<int>()),
            "known type #()");
    EXPECT_EQ(type_string("known type %1%",
                    "unknown type",
                    vector< vector<int> >()),
            "known type #()");
}
