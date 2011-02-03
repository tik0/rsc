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

#include <rsc/runtime/Demangle.h>

using namespace std;
using namespace rsc::runtime;

TEST(DemangleTest, testDemangle)
{
    EXPECT_EQ(demangle(typeid(bool).name()), "bool");
    EXPECT_EQ(demangle(typeid(int).name()), "int");
    EXPECT_EQ(demangle(typeid(std::string).name()), "std::string");
    EXPECT_EQ(demangle(typeid(std::vector<int>).name()), "std::vector<int, std::allocator<int> >");
}
