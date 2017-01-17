/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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
#include <iostream>

#include <boost/static_assert.hpp>

#include <gtest/gtest.h>

#include <rsc/debug/DebugTools.h>

using namespace std;
using namespace rsc::debug;

TEST(DebugToolsTest, testCreateBacktrace)
{
    vector<string> trace = createBacktrace();
    for (vector<string>::const_iterator it = trace.begin(); it != trace.end(); ++it) {
        cout << *it << endl;
    }
}

TEST(DebugToolsTest, testExceptionInfo)
{

    try {
        throw invalid_argument("This is a message");
    } catch (exception& e) {
        cout << exceptionInfo(e);
    }

}
