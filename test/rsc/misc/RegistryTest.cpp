/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <stdexcept>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rsc/misc/Registry.h"
#include "rsc/misc/langutils.h"

#include "testRegistry.h"
#include "RegistreeLib.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

rsc::misc::Registry<TestRegistree>* testRegistry() {
    static rsc::misc::Registry< TestRegistree >* registry = new ::rsc::misc::Registry<TestRegistree>;
    return registry;
}

static string getGlobalKey1() {
    return "global1";
}
static string getGlobalKey2() {
    return "global2";
}

CREATE_GLOBAL_REGISTREE(globalTestRegistry(), new TestRegistree(getGlobalKey1()), global1)
;
CREATE_GLOBAL_REGISTREE(globalTestRegistry(), new TestRegistree(getGlobalKey2()), global2)
;

TEST(RegistryTest, testGlobalRegistration) {
	// TODO XXX the global registration works only if the dll is somehow used on windows
	cout << getGlobalKey3() << endl;
    EXPECT_EQ((size_t) 3, globalTestRegistry()->getKnownRegistryKeys().size());
}

TEST(RegistryTest, testRegistration)
{

    string dummyName1 = randAlnumStr(20);
    EXPECT_THROW(testRegistry()->getRegistree(dummyName1), invalid_argument);
    EXPECT_TRUE(testRegistry()->getKnownRegistryKeys().empty());

    TestRegistree* f = new TestRegistree(dummyName1);
    testRegistry()->addRegistree(f);
    EXPECT_EQ(f, testRegistry()->getRegistree(dummyName1).get());

    EXPECT_TRUE(testRegistry()->getKnownRegistryKeys().count(dummyName1) > 0);
    EXPECT_EQ((size_t) 1, testRegistry()->getKnownRegistryKeys().size());

    TestRegistree* f2 = new TestRegistree(dummyName1);
    EXPECT_THROW(testRegistry()->addRegistree(f2), invalid_argument);
    delete f2;

}

TEST(RegistryTest, testRemove)
{

    string dummyName1 = randAlnumStr(20);
    TestRegistree* r = new TestRegistree(dummyName1);
    testRegistry()->addRegistree(r);
    EXPECT_TRUE(testRegistry()->getKnownRegistryKeys().count(dummyName1) > 0);
    EXPECT_TRUE(testRegistry()->removeRegistree(dummyName1));
    EXPECT_FALSE(testRegistry()->getKnownRegistryKeys().count(dummyName1) > 0);
    EXPECT_FALSE(testRegistry()->removeRegistree(dummyName1));

}
