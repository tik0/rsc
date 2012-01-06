/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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

#include <string>
#include <vector>

#include <boost/static_assert.hpp>

#include <gtest/gtest.h>

#include <rsc/runtime/TypeStringTools.h>

using namespace std;
using namespace rsc::runtime;

TEST(TypeStringToolsTest, testTypeName)
{

#if defined DEMANGLE_GCC
    string stringName = "std::string";
#elif defined DEMANGLE_MSVC
    string stringName = "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >";
#else
    BOOST_STATIC_ASSERT(false);
#endif

    EXPECT_EQ("bool", typeName(typeid(bool)));
    EXPECT_EQ("int", typeName(typeid(int)));
    EXPECT_EQ(stringName, typeName(typeid(string)));

    EXPECT_EQ("bool", typeName<bool>());
    EXPECT_EQ("int", typeName<int>());
    EXPECT_EQ(stringName, typeName<string>());

    EXPECT_EQ("bool", typeName(true));
    EXPECT_EQ("int", typeName(1));
    EXPECT_EQ(stringName, typeName(string("bla")));
}

TEST(TypeStringToolsTest, testTypeString)
{
    EXPECT_EQ(typeString("known type %1%",
                    "unknown type",
                    true),
            "known type 1");
    EXPECT_EQ(typeString("known type %1%",
                    "unknown type",
                    5),
            "known type 5");
    EXPECT_EQ(typeString("known type %1%",
                    "unknown type",
                    string("test")),
            "known type test");
    EXPECT_EQ(typeString("known type %1%",
                    "unknown type",
                    vector<int>()),
            "known type #()");
    EXPECT_EQ(typeString("known type %1%",
                    "unknown type",
                    vector< vector<int> >()),
            "known type #()");
}
