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
#include <vector>

#include <boost/bind.hpp>

#include <gtest/gtest.h>

#include <rsc/patterns/ObservableFactory.h>

#include "mocks.h"

using namespace std;
using namespace boost;
using namespace rsc::runtime;
using namespace rsc::patterns;

typedef ObservableFactory<string, Interface> TestFactory;

class ObservableFactoryTest: public ::testing::Test {
protected:
    TestFactory factory;

    virtual void SetUp() {
        this->factory.impls().register_("Impl1", &Impl1::create);
        this->factory.impls().register_("Impl2", &Impl2::create);
    }
};

TEST_F(ObservableFactoryTest, testSignals) {
    vector<string> added;
    vector<string> removed;

    // as with tr1 vector now has an overloaded push_back method, we need to
    // convince bind to use a specific version (e.g. for msvc 2010)
    factory.signalImplAdded().connect(
            boost::bind(static_cast<void(std::vector<string>::*)(const string&)>
            (&std::vector<string>::push_back), boost::ref(
                    added), _1));
    factory.signalImplRemoved().connect(
            boost::bind(static_cast<void(std::vector<string>::*)(const string&)>
            (&std::vector<string>::push_back), boost::ref(
                    removed), _1));

    factory.impls().unregister("Impl1");
    EXPECT_EQ(added.size(), size_t(0));
    EXPECT_EQ(removed.size(), size_t(1));
    EXPECT_EQ(removed.back(), "Impl1");

    factory.impls().register_("Impl1", &Impl1::create);
    EXPECT_EQ(added.size(), size_t(1));
    EXPECT_EQ(added.back(), "Impl1");
    EXPECT_EQ(removed.size(), size_t(1));
}
