/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2011 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
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

#include <sstream>

#include <boost/shared_ptr.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/runtime/Printable.h"
#include "rsc/misc/langutils.h"

using namespace std;
using namespace rsc::runtime;
using namespace testing;

class DummyPrintable: public Printable {
public:

    DummyPrintable(const string& name) :
        name(name) {
    }

    string getClassName() const {
        return name;
    }

    void printContents(ostream& stream) const {
        stream << "CONTENTS";
    }

private:
    string name;

};

TEST(PrintableTest, testPrint)
{

    const string name = "AName";
    DummyPrintable p(name);
    {
        stringstream s;
        p.print(s);
        EXPECT_EQ(name + "[CONTENTS]", s.str());
    }

    {
        stringstream s;
        s << p;
        EXPECT_EQ(name + "[CONTENTS]", s.str());
    }

    {
        stringstream s;
        boost::shared_ptr<DummyPrintable> ptr(&p, rsc::misc::NullDeleter());
        s << ptr;
        EXPECT_EQ("*" + name + "[CONTENTS] at ", s.str().substr(0, 20));
    }

    {
        stringstream s;
        boost::shared_ptr<Printable> ptr(&p, rsc::misc::NullDeleter());
        s << ptr;
        EXPECT_EQ("*" + name + "[CONTENTS] at ", s.str().substr(0, 20));
    }

}
