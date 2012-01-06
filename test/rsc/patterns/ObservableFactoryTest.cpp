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

#include <stdexcept>
#include <string>
#include <vector>

#include <boost/bind.hpp>

#include <gtest/gtest.h>

#include <rsc/patterns/ObservableFactory.h>

#include "mocks.h"

using namespace std;
using namespace boost;
using namespace rsc::runtime;
using namespace rsc::patterns;

typedef ObservableFactory<string, Interface> TestFactory;

class ObservableFactoryTest: public ::testing::Test {
protected:
    TestFactory factory;

    virtual void SetUp() {
        this->factory.impls().register_("Impl1", &Impl1::create);
        this->factory.impls().register_("Impl2", &Impl2::create);
    }
};

TEST_F(ObservableFactoryTest, testSignals) {
    vector<string> added;
    vector<string> removed;

    // as with tr1 vector now has an overloaded push_back method, we need to
    // convince bind to use a specific version (e.g. for msvc 2010)
    factory.signalImplAdded().connect(
            boost::bind(static_cast<void(std::vector<string>::*)(const string&)>
            (&std::vector<string>::push_back), boost::ref(
                    added), _1));
    factory.signalImplRemoved().connect(
            boost::bind(static_cast<void(std::vector<string>::*)(const string&)>
            (&std::vector<string>::push_back), boost::ref(
                    removed), _1));

    factory.impls().unregister("Impl1");
    EXPECT_EQ(added.size(), size_t(0));
    EXPECT_EQ(removed.size(), size_t(1));
    EXPECT_EQ(removed.back(), "Impl1");

    factory.impls().register_("Impl1", &Impl1::create);
    EXPECT_EQ(added.size(), size_t(1));
    EXPECT_EQ(added.back(), "Impl1");
    EXPECT_EQ(removed.size(), size_t(1));
}
