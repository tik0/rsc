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
#include <iostream>

#include <boost/static_assert.hpp>

#include <gtest/gtest.h>

#include <rsc/runtime/Demangle.h>

using namespace std;
using namespace rsc::runtime;

TEST(DemangleTest, testDemangle)
{
    EXPECT_EQ("bool", demangle(typeid(bool).name()));
    EXPECT_EQ("int", demangle(typeid(int).name()));
#if defined DEMANGLE_GCC
    EXPECT_EQ("std::string", demangle(typeid(std::string).name()));
    EXPECT_EQ("std::vector<int, std::allocator<int> >", demangle(typeid(std::vector<int>).name()));
#elif defined DEMANGLE_MSVC
    EXPECT_EQ("class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", demangle(typeid(std::string).name()));
    EXPECT_EQ("class std::vector<int,class std::allocator<int> >", demangle(typeid(std::vector<int>).name()));
#else
    BOOST_STATIC_ASSERT(false);
#endif
}
