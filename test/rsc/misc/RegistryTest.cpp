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

#include "rsc/misc/Registry.h"
#include "rsc/misc/langutils.h"

#include "testRegistry.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

CREATE_REGISTRY(TestRegistree, testRegistry)
;

static string getGlobalKey1() {
    return "global1";
}
static string getGlobalKey2() {
    return "global2";
}

CREATE_GLOBAL_REGISTREE(globalTestRegistry(), new TestRegistree(getGlobalKey1()), global1)
;
CREATE_GLOBAL_REGISTREE(globalTestRegistry(), new TestRegistree(getGlobalKey2()), global2)
;

TEST(RegistryTest, testGlobalRegistration) {
    EXPECT_EQ((size_t) 3, globalTestRegistry()->getKnownRegistryKeys().size());
}

TEST(RegistryTest, testRegistration)
{

    string dummyName1 = randAlnumStr(20);
    EXPECT_THROW(testRegistry()->getRegistree(dummyName1), invalid_argument);
    EXPECT_TRUE(testRegistry()->getKnownRegistryKeys().empty());

    TestRegistree *f = new TestRegistree(dummyName1);
    testRegistry()->addRegistree(f);
    EXPECT_EQ(f, testRegistry()->getRegistree(dummyName1).get());

    EXPECT_TRUE(testRegistry()->getKnownRegistryKeys().count(dummyName1));
    EXPECT_EQ((size_t) 1, testRegistry()->getKnownRegistryKeys().size());

    TestRegistree *f2 = new TestRegistree(dummyName1);
    EXPECT_THROW(testRegistry()->addRegistree(f2), invalid_argument);
    delete f2;

}

TEST(RegistryTest, testRemove)
{

    string dummyName1 = randAlnumStr(20);
    TestRegistree *r = new TestRegistree(dummyName1);
    testRegistry()->addRegistree(r);
    EXPECT_TRUE(testRegistry()->getKnownRegistryKeys().count(dummyName1));
    EXPECT_TRUE(testRegistry()->removeRegistree(dummyName1));
    EXPECT_FALSE(testRegistry()->getKnownRegistryKeys().count(dummyName1));
    EXPECT_FALSE(testRegistry()->removeRegistree(dummyName1));

}
