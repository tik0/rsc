/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "rsc/misc/langutils.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

DEPRECATED(void deprecatedFunction(int foo));
void deprecatedFunction(int foo) {
    int bla = foo + 1;
    bla = bla + 1;
}

class Foo {
public:
    DEPRECATED(void deprecatedFunction(int foo)) {
        int bla = foo + 1;
        bla = bla + 1;
    }
};

DEPRECATED_MSG(void deprecatedFunctionMsg(int foo), "A test deprecation");
void deprecatedFunctionMsg(int foo) {
    int bla = foo + 1;
    bla = bla + 1;
}

class DEPRECATED_CLASS("Test deprecation for a simple class") SimpleDeprecation;
class SimpleDeprecation {
};

template<class T>
class DEPRECATED_CLASS("Test deprecation for a template class") TemplateDeprecation;

template<class T>
class TemplateDeprecation {
    std::pair<T, T> doSomething() {
        return std::pair<T, T>();
    }
};

TEST(DeprecationTest, testDeprecation) {
    deprecatedFunction(42);
    deprecatedFunctionMsg(42);
    Foo().deprecatedFunction(42);
    SimpleDeprecation s;
    (void) s;
    TemplateDeprecation<int> t;
    (void) t;
}
