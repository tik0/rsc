/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include <fstream>

#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <rsc/config/OptionHandler.h>
#include <rsc/config/ConfigFileSource.h>
#include <rsc/config/TypedValue.h>

#include "testconfig.h"

using namespace std;
using namespace boost;
using namespace rsc::config;

class DummyHandler : public OptionHandler {
public:
    void handleOption(const vector<string> &/*key*/, const string &value) {
        parseTypedValue(value);
    }
};

TEST(ConfigFileSourceTest, testSyntaxErrors)
{
    DummyHandler handler;
    for (unsigned int i = 1; i <= 3; ++i) {
        ifstream stream(str(format("%1%/syntax-errors-%2%.conf")
	                % TEST_ROOT % i).c_str());
	ConfigFileSource source(stream);
	EXPECT_THROW(source.emit(handler), invalid_argument);
    }
}
