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
#include <rsc/runtime/Properties.h>

#include "testconfig.h"

using namespace std;
using namespace boost;
using namespace rsc::config;
using namespace rsc::runtime;

class CollectingHandler: public OptionHandler {
public:
    void handleOption(const vector<string>& key, const string& value) {
        string name;
        for (vector<string>::const_iterator it = key.begin(); it != key.end(); ++it) {
            if (!(it == key.begin())) {
                name += ".";
            }
            name += *it;
        }
        this->collected[name] = parseTypedValue(value);
    }

    // TODO jwienke: for some strange reasons, on windows we have to use
    // Properties here and not map<string, any>, because otherwise there will be
    // a linker error. See bug #784 for more details.
    Properties collected;
};

class ConfigFileSourceEncodingTest : public ::testing::TestWithParam<string> {
};


TEST_P(ConfigFileSourceEncodingTest, testSmoke)
{

    CollectingHandler handler;
    ifstream stream(str(format("%1%/%2%") % TEST_ROOT % GetParam()).c_str());
    ConfigFileSource source(stream);
    source.provideOptions(handler);
    EXPECT_EQ(any_cast<int>(handler.collected["global"]), 5);
    EXPECT_EQ(any_cast<string>(handler.collected["string"]), "string");
    EXPECT_EQ(any_cast<double>(handler.collected["section1.option"]), 1.5);
}

INSTANTIATE_TEST_CASE_P(ValidFiles,
                        ConfigFileSourceEncodingTest,
                        ::testing::Values("smoke-unix.conf", "smoke-windows.conf", "smoke-mac.conf"));

TEST(ConfigFileSourceTest, testSyntaxErrors)
{
    CollectingHandler handler;
    for (unsigned int i = 1; i <= 3; ++i) {
        ifstream
                stream(
                        str(
                                format("%1%/syntax-errors-%2%.conf")
                                        % TEST_ROOT % i).c_str());
        ConfigFileSource source(stream);
        EXPECT_THROW(source.provideOptions(handler), invalid_argument);
    }
}
