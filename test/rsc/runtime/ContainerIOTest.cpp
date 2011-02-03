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

#include <utility>
#include <list>
#include <vector>
#include <set>
#include <map>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

#include <rsc/runtime/ContainerIO.h>

using namespace std;
using namespace boost;

class ContainerIOTest : public ::testing::Test {
public:
  pair<double, string> pair(5.0, "a");
  list<int>            list;
  vector<int>          vector;
  set<int>             set;
  map<string, double>  map;

  virtual void
  SetUp() {
    this->list.clear();
    this->list.push_back(1);
    this->list.push_back(2);
    this->list.push_back(3);
    this->list.push_back(4);

    this->vector.clear();
    this->vector.push_back(1);
    this->vector.push_back(2);
    this->vector.push_back(3);
    this->vector.push_back(4);

    this->set.clear();
    this->set.insert(1);
    this->set.insert(2);
    this->set.insert(3);
    this->set.insert(4);

    this->map.clear();
    this->map.insert(make_pair("a", 1.0));
    this->map.insert(make_pair("b", 2.0));
    this->map.insert(make_pair("c", 3.0));
  }
};

TEST(ContainerIOTest, testOutput) {
  EXPECT_EQ(lexical_cast<std::string>(this->list),
	    "[1, 2, 3, 4]");
  EXPECT_EQ(demangle(typeid(int).name()),              "int");
  EXPECT_EQ(demangle(typeid(std::string).name()),      "std::string");
  EXPECT_EQ(demangle(typeid(std::vector<int>).name()), "std::vector<int>");
}
