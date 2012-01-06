/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <stdexcept>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rsc/misc/UUID.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

TEST(UUIDTest, testRandomCreate)
{
    EXPECT_TRUE(UUID().getId() != UUID().getId());
}

TEST(UUIDTest, testByteCreate)
{
    UUID id1;
    UUID id2(id1.getId().data);
    EXPECT_EQ(id1, id2);
}

TEST(UUIDTest, testNameCreate)
{
    UUID ns("d37bae80-b279-11e0-a06a-001aa0342d7d");
    UUID id1(ns, "my name");
    UUID id2(ns, "my name");
    EXPECT_EQ(id1, id2);
    UUID id3(ns, "some other name");
    EXPECT_NE(id1, id3);
}

TEST(UUIDTest, testComparisonOperators)
{

    UUID id1;
    UUID id2;
    EXPECT_EQ(id1, id1);
    EXPECT_NE(id2, id1);
    EXPECT_FALSE(id1 < id1);

}

TEST(UUIDTest, testStringConstructor)
{

    UUID id;
    UUID parsed(id.getIdAsString());
    EXPECT_EQ(id, parsed);

    EXPECT_THROW(UUID("I AM NOT AN ID"), runtime_error);

}
