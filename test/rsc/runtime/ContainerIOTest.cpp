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
  pair<double, string> pair_;
  list<int>            list_;
  vector<int>          vector_;
  set<int>             set_;
  map<string, double>  map_;

  virtual void
  SetUp() {
    this->pair_.first  = 5.0;
    this->pair_.second = "a";

    this->list_.clear();
    this->list_.push_back(1);
    this->list_.push_back(2);
    this->list_.push_back(3);
    this->list_.push_back(4);

    this->vector_.clear();
    this->vector_.push_back(1);
    this->vector_.push_back(2);
    this->vector_.push_back(3);
    this->vector_.push_back(4);

    this->set_.clear();
    this->set_.insert(1);
    this->set_.insert(2);
    this->set_.insert(3);
    this->set_.insert(4);

    this->map_.clear();
    this->map_.insert(make_pair("a", 1.0));
    this->map_.insert(make_pair("b", 2.0));
    this->map_.insert(make_pair("c", 3.0));
  }
};

TEST_F(ContainerIOTest, testOutput) {
  EXPECT_EQ(lexical_cast<std::string>(this->list_),
	    "[1, 2, 3, 4]");
}
