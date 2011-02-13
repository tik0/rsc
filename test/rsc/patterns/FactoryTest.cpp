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

#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include <rsc/patterns/Factory.h>

#include "mocks.h"

using namespace std;
using namespace boost;
using namespace rsc::runtime;
using namespace rsc::patterns;

typedef Factory<string, Interface> TestFactory;

class FactoryTest: public ::testing::Test {
protected:
    TestFactory factory;

    virtual void SetUp() {
        this->factory.impls().register_("Impl1", &Impl1::create);
        this->factory.impls().register_("Impl2", &Impl2::create);
        this->factory.impls().register_("ImplFailingConstructor",
                &ImplFailingConstructor::create);
    }
};

TEST_F(FactoryTest, testRegistration)
{
    TestFactory factory;

    // Should start out empty
    EXPECT_EQ(factory.impls().size(), size_t(0));

    // Ordinary registration
    factory.impls().register_("Impl1", &Impl1::create);
    factory.impls().register_("Impl2", &Impl2::create);
    factory.impls().register_("ImplFailingConstructor",
            &ImplFailingConstructor::create);
    EXPECT_EQ(factory.impls().size(), size_t(3));

    // Duplicates
    factory.impls().register_("duplicate", &Impl1::create);
    EXPECT_THROW(factory.impls().register_("duplicate", &Impl1::create),
            invalid_argument);

    // Ordinary Unregistration
    factory.impls().unregister("Impl1");
    EXPECT_EQ(factory.impls().size(), size_t(3));

    // Invalid Unregistration
    EXPECT_THROW(factory.impls().unregister("Impl1"),
            NoSuchImpl);
}

TEST_F(FactoryTest, testCreation)
{
    // Non-existent impl
    EXPECT_THROW(factory.createInst("ImplDoesNotExist"),
            NoSuchImpl);

    // Ordinary creation
    {
        Properties p;
        p["string_param"] = string("test");
        p["float_param"] = static_cast<float>(1.0);
        Interface* instance = factory.createInst("Impl1", p);
        EXPECT_TRUE(instance != 0);
    }

    {
        Interface* instance = factory.createInst("Impl2");
        EXPECT_TRUE(instance != 0);
    }

    // Constructor error
    EXPECT_THROW(factory.createInst("ImplFailingConstructor"),
            ConstructError);
}
