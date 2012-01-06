/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
 *
 * This file may be licensed under the terms of of the
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
