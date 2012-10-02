/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011, 2012 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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
#include <string>

#include <gtest/gtest.h>

#include <boost/thread.hpp>

#include <rsc/patterns/Singleton.h>

using namespace std;
using namespace rsc::patterns;

class LongConstructionThing: public Singleton<LongConstructionThing> {
    friend class Singleton<LongConstructionThing>;

    LongConstructionThing() {
        {
            boost::mutex::scoped_lock lock(constructionMutex);
            ++constructionCount;
        }
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }

public:

    virtual ~LongConstructionThing() {
        boost::mutex::scoped_lock lock(constructionMutex);
        ++destructionCount;
    }

    static boost::mutex constructionMutex;
    static int constructionCount;
    static int destructionCount;

};

boost::mutex LongConstructionThing::constructionMutex;
int LongConstructionThing::constructionCount;
int LongConstructionThing::destructionCount;

class SingletonTest: public ::testing::Test {
protected:

    virtual void SetUp() {
        LongConstructionThing::killInstance();
        LongConstructionThing::constructionCount = 0;
        LongConstructionThing::destructionCount = 0;
    }
};

TEST_F(SingletonTest, testConcurrentGetInstance)
{

    boost::thread thread1(&LongConstructionThing::getInstance);
    boost::thread thread2(&LongConstructionThing::getInstance);

    thread1.join();
    thread2.join();

    EXPECT_EQ(1, LongConstructionThing::constructionCount);

}

TEST_F(SingletonTest, testKillInstance)
{

    LongConstructionThing::getInstance();

    LongConstructionThing::killInstance();
    EXPECT_EQ(1, LongConstructionThing::destructionCount);
    LongConstructionThing::killInstance();
    EXPECT_EQ(1, LongConstructionThing::destructionCount);

    LongConstructionThing::getInstance();
    LongConstructionThing::killInstance();
    EXPECT_EQ(2, LongConstructionThing::destructionCount);

}
