/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2013 Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
 *
 * ============================================================ */

#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <rsc/config/OptionHandler.h>
#include <rsc/config/CommandLinePropertySource.h>
#include <rsc/config/CollectingOptionHandler.h>

#include "testconfig.h"

using namespace std;
using namespace boost;
using namespace rsc::config;
using namespace rsc::runtime;

// Avoid warnings about deprecated use of character strings for the common
// construction of argv arrays.
#pragma GCC diagnostic ignored "-Wwrite-strings"

class CommandLinePropertySourceTest: public testing::Test {
public:
    void SetUp() {
        handler = CollectingOptionHandler();
    }

    Properties getOptions(int argc, char** argv, bool report = false) {
        CommandLinePropertySource source(argc, argv, report);
        source.provideOptions(handler);
        return handler.getOptions();
    }

    CollectingOptionHandler handler;

};

TEST_F(CommandLinePropertySourceTest, testSmoke) {

    int argc = 5;
    char* argv[] = { "testProgram", "-Dfoo.bar=42", "-Dnarf=a test string",
            "-D", "blub=111" };

    Properties options = getOptions(argc, argv);
    EXPECT_EQ(42, options.getAs<int>("foo.bar"));
    EXPECT_EQ("a test string", options.get<string>("narf"));
    EXPECT_EQ(111, options.getAs<int>("blub"));

}

TEST_F(CommandLinePropertySourceTest, testIgnoreUnknownOptions) {

    int argc = 3;
    char* argv[] = { "testProgram", "--switch", "-Dnarf=a test string" };

    Properties options = getOptions(argc, argv);
    EXPECT_EQ("a test string", options.get<string>("narf"));

}

TEST_F(CommandLinePropertySourceTest, testEmptyValue) {

    int argc = 2;
    char* argv[] = { "testProgram", "-Da.test=" };

    Properties options = getOptions(argc, argv);
    EXPECT_EQ("", options.get<string>("a.test"));

}

TEST_F(CommandLinePropertySourceTest, testIgnoreInvalidSyntax) {

    int argc = 7;
    char* argv[] = { "testProgram", "-Dno.equals", "-D=noKey", "-D",
            "-Dkey..with.multiple=dots", "-Ddots.at.=end", "-D.dots.at=begin" };

    Properties options = getOptions(argc, argv);
    EXPECT_EQ(size_t(0), options.size());

}

TEST_F(CommandLinePropertySourceTest, testReportInvalidSyntax) {

    vector<string> invalidArguments;
    invalidArguments.push_back("-Dno.equals");
    invalidArguments.push_back("-D=noKey");
    invalidArguments.push_back("-D");
    invalidArguments.push_back("-Dkey..with.multiple=dots");
    invalidArguments.push_back("-Ddots.at.=end");
    invalidArguments.push_back("-D.dots.at=begin");

    for (vector<string>::const_iterator argIt = invalidArguments.begin();
            argIt != invalidArguments.end(); ++argIt) {
        int argc = 2;
        char* argv[] = { "testProgram", const_cast<char*>(argIt->c_str()) };
        EXPECT_THROW(getOptions(argc, argv, true), invalid_argument);
    }

}

TEST_F(CommandLinePropertySourceTest, testDifferentOptionKey) {

    int argc = 2;
    char* argv[] = { "testProgram", "-Xfoo=bar" };

    CommandLinePropertySource source(argc, argv, false, 'X');
    source.provideOptions(handler);
    EXPECT_EQ("bar", handler.getOptions().get<string>("foo"));

}

TEST_F(CommandLinePropertySourceTest, testPositionalArgumentsDelimiter) {

    int argc = 4;
    char* argv[] = { "testProgram", "-Dfoo=bar",  "--", "-Dtest=nothing"};

    CommandLinePropertySource source(argc, argv);
    source.provideOptions(handler);
    EXPECT_EQ("bar", handler.getOptions().get<string>("foo"));
    EXPECT_EQ(0, handler.getOptions().count("test"));

}

#pragma GCC diagnostic pop
