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

typedef Factory<string, interface> test_factory;

class FactoryTest: public ::testing::Test {
protected:
    test_factory factory;

    virtual void SetUp() {
        this->factory.impls().register_("impl_1", &impl_1::create);
        this->factory.impls().register_("impl_2", &impl_2::create);
        this->factory.impls().register_("impl_failing_constructor",
                &impl_failing_constructor::create);
    }
};

TEST_F(FactoryTest, testRegistration)
{
    test_factory factory;

    // Should start out empty
    EXPECT_EQ(factory.impls().size(), size_t(0));

    // Ordinary registration
    factory.impls().register_("impl_1", &impl_1::create);
    factory.impls().register_("impl_2", &impl_2::create);
    factory.impls().register_("impl_failing_constructor",
            &impl_failing_constructor::create);
    EXPECT_EQ(factory.impls().size(), 3);

    // Duplicates
    factory.impls().register_("duplicate", &impl_1::create);
    EXPECT_THROW(factory.impls().register_("duplicate", &impl_1::create),
            invalid_argument);

    // Ordinary Unregistration
    factory.impls().unregister("impl_1");
    EXPECT_EQ(factory.impls().size(), 3);

    // Invalid Unregistration
    EXPECT_THROW(factory.impls().unregister("impl_1"),
            NoSuchImpl);
}

TEST_F(FactoryTest, testCreation)
{
    // Non-existent impl
    EXPECT_THROW(factory.create_inst("impl_does_not_exist"),
            NoSuchImpl);

    // Ordinary creation
    {
        Properties p;
        p["string_param"] = string("test");
        p["float_param"] = static_cast<float>(1.0);
        interface* instance = factory.create_inst("impl_1", p);
        EXPECT_TRUE(instance);
    }

    {
        interface* instance = factory.create_inst("impl_2");
        EXPECT_TRUE(instance);
    }

    // Constructor error
    EXPECT_THROW(factory.create_inst("impl_failing_constructor"),
            ConstructError);
}
