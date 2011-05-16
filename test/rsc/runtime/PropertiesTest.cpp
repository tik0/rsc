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

#include <gtest/gtest.h>

#include <rsc/runtime/Properties.h>

using namespace std;
using namespace boost;

using namespace rsc::runtime;

class PropertiesTest: public ::testing::Test {
public:
    Properties p1;
    Properties p2;
    Properties onlyStrings;

    virtual void SetUp() {
	this->p1["foo"] = 1;
	this->p1["bar"] = string("baz");

        this->p2["foo"] = 2;
        this->p2["woob"] = 1.5;

        this->onlyStrings["foo"] = string("1");
	this->onlyStrings["bar"] = string("baz");
        this->onlyStrings["woob"] = string("1.5");
    }
};

TEST_F(PropertiesTest, testGetAs)
{
    EXPECT_EQ(onlyStrings.getAs<int>   ("foo"),  1);
    EXPECT_EQ(onlyStrings.getAs<string>("bar"),  "baz");
    EXPECT_EQ(onlyStrings.getAs<double>("woob"), 1.5);

    EXPECT_EQ(onlyStrings.getAs<double>("no-such-property", 5.0), 5.0);

    EXPECT_THROW(onlyStrings.getAs<double>("no-such-property"), NoSuchObject);

    EXPECT_THROW(onlyStrings.getAs<double>("bar"), bad_cast);
}

TEST_F(PropertiesTest, testMerge)
{
    {
        Properties merged = p1 << p2;
        EXPECT_EQ(merged.get<int>("foo"), 2);
        EXPECT_EQ(merged.get<string>("bar"), "baz");
        EXPECT_EQ(merged.get<double>("woob"), 1.5);
    }

    {
        Properties merged = p1;
        merged <<= p2;
        EXPECT_EQ(merged.get<int>("foo"), 2);
        EXPECT_EQ(merged.get<string>("bar"), "baz");
        EXPECT_EQ(merged.get<double>("woob"), 1.5);
    }
}

TEST_F(PropertiesTest, testCompare)
{
    EXPECT_EQ(p1, p1);
    EXPECT_EQ(p2, p2);
    EXPECT_NE(p1, p2);
    EXPECT_NE(p2, p1);
}
