/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011, 2014 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include <fstream>

#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <rsc/config/OptionHandler.h>
#include <rsc/config/ConfigFileSource.h>
#include <rsc/config/CollectingOptionHandler.h>
#include <rsc/runtime/Properties.h>

#include "testconfig.h"

using namespace std;
using namespace boost;
using namespace rsc::config;
using namespace rsc::runtime;

class ConfigFileSourceEncodingTest : public ::testing::TestWithParam<string> {
};


TEST_P(ConfigFileSourceEncodingTest, testSmoke)
{
    ifstream stream(str(format("%1%/%2%") % TEST_ROOT % GetParam()).c_str());
    ConfigFileSource source(stream);

    // Test multiple iterations to be sure that the object can be
    // reused for multiple handlers.
    for (unsigned int i = 0; i < 3; ++i) {
        CollectingOptionHandler handler;
        source.provideOptions(handler);
        EXPECT_EQ(any_cast<string>(handler.getOptions()["global"]), "5");
        EXPECT_EQ(any_cast<string>(handler.getOptions()["string"]), "string");
        EXPECT_EQ(any_cast<string>(handler.getOptions()["section1.option"]), "1.5");
    }
}

INSTANTIATE_TEST_CASE_P(ValidFiles,
                        ConfigFileSourceEncodingTest,
                        ::testing::Values("smoke-unix.conf", "smoke-windows.conf"));

TEST(ConfigFileSourceTest, testSyntaxErrors) {
    CollectingOptionHandler handler;
    for (unsigned int i = 1; i <= 2; ++i) {
        ifstream stream(
                str(format("%1%/syntax-errors-%2%.conf") % TEST_ROOT % i).c_str());
        try {
            ConfigFileSource source(stream);
            source.provideOptions(handler);
            FAIL();
        } catch (invalid_argument& e) {
            // good
        }
    }
}

TEST(ConfigFileSourceTest, testEolError) {
    CollectingOptionHandler handler;
    ifstream stream(str(format("%1%/smoke-mac.conf") % TEST_ROOT).c_str());
    try {
        ConfigFileSource source(stream);
        source.provideOptions(handler);
        FAIL();
    } catch (invalid_argument& e) {
        // good
    }
}
