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

using namespace std;
using namespace testing;
using namespace rsc::misc;

class TestRegistree {
public:
    string key;
    TestRegistree(const string &key) :
        key(key) {

    }
    string getRegistryKey() const {
        return key;
    }
};

CREATE_REGISTRY(TestRegistree);

TEST(RegistryTest, testInstance)
{
    EXPECT_EQ(Registry<TestRegistree>::instance(), Registry<TestRegistree>::instance());
}

TEST(RegistryTest, testRegistration)
{

    string dummyName1 = randAlnumStr(20);
    EXPECT_THROW(Registry<TestRegistree>::instance()->getRegistree(dummyName1), invalid_argument);
    EXPECT_TRUE(Registry<TestRegistree>::instance()->getKnownRegistryKeys().empty());

    TestRegistree *f = new TestRegistree(dummyName1);
    Registry<TestRegistree>::instance()->addRegistree(f);
    EXPECT_EQ(f, Registry<TestRegistree>::instance()->getRegistree(dummyName1).get());

    EXPECT_TRUE(Registry<TestRegistree>::instance()->getKnownRegistryKeys().count(dummyName1));
    EXPECT_EQ((size_t) 1, Registry<TestRegistree>::instance()->getKnownRegistryKeys().size());

    TestRegistree *f2 = new TestRegistree(dummyName1);
    EXPECT_THROW(Registry<TestRegistree>::instance()->addRegistree(f2), invalid_argument);
    delete f2;

}

TEST(RegistryTest, testRemove)
{

    string dummyName1 = randAlnumStr(20);
    TestRegistree *r = new TestRegistree(dummyName1);
    Registry<TestRegistree>::instance()->addRegistree(r);
    EXPECT_TRUE(Registry<TestRegistree>::instance()->getKnownRegistryKeys().count(dummyName1));
    EXPECT_TRUE(Registry<TestRegistree>::instance()->removeRegistree(dummyName1));
    EXPECT_FALSE(Registry<TestRegistree>::instance()->getKnownRegistryKeys().count(dummyName1));
    EXPECT_FALSE(Registry<TestRegistree>::instance()->removeRegistree(dummyName1));

}
