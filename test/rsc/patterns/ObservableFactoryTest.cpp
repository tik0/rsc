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

typedef ObservableFactory<string, interface> test_factory;

class ObservableFactoryTest : public ::testing::Test {
protected:
  test_factory factory;

  virtual void
  SetUp() {
    this->factory.impls().register_("impl_1",  &impl_1::create);
    this->factory.impls().register_("impl_2",  &impl_2::create);
    this->factory.impls().register_("impl_failing_constructor",
				    &impl_failing_constructor::create);
  }
};

TEST_F(ObservableFactoryTest, testSignals) {
  vector<string> added;
  vector<string> removed;

  factory.signal_impl_added().connect(bind(&vector<string>::push_back,
					   ref(added), _1));
  factory.signal_impl_removed().connect(bind(&vector<string>::push_back,
					     ref(removed), _1));

  factory.impls().unregister("impl_1");
  EXPECT_EQ(added.size(), size_t(0));
  EXPECT_EQ(removed.size(), size_t(1));
  EXPECT_EQ(removed.back(), "impl_1");

  factory.impls().register_("impl_1", &impl_1::create);
  EXPECT_EQ(added.size(), size_t(1));
  EXPECT_EQ(added.back(), "impl_1");
  EXPECT_EQ(removed.size(), size_t(1));
}
