/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "rsc/misc/langutils.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

DEPRECATED(void deprecatedFunction(int foo));
void deprecatedFunction(int foo) {
    int bla = foo + 1;
    bla = bla + 1;
}

class Foo {
public:
    DEPRECATED(void deprecatedFunction(int foo)) {
        int bla = foo + 1;
        bla = bla + 1;
    }
};

TEST(DeprecationTest, testDeprecation) {
    deprecatedFunction(42);
    Foo().deprecatedFunction(42);
}
