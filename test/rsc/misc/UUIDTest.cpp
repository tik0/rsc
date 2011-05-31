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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rsc/misc/UUID.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

TEST(UUIDTest, testRandomCreate)
{
    EXPECT_TRUE(UUID().getId() != UUID().getId());
}

TEST(UUIDTest, testByteCreate)
{
    UUID id1;
    UUID id2(id1.getId().data);
    EXPECT_EQ(id1, id2);
}

TEST(UUIDTest, testComparisonOperators)
{

    UUID id1;
    UUID id2;
    EXPECT_EQ(id1, id1);
    EXPECT_NE(id2, id1);
    EXPECT_FALSE(id1 < id1);

}

TEST(UUIDTest, testStringConstructor)
{

    UUID id;
    UUID parsed(id.getIdAsString());
    EXPECT_EQ(id, parsed);

    EXPECT_THROW(UUID("I AM NOT AN ID"), runtime_error);

}
