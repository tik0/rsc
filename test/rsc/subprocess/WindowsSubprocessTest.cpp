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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/thread.hpp>

#include "rsc/subprocess/WindowsSubprocess.h"

#include "testconfig.h"

using namespace std;
using namespace testing;
using namespace rsc;
using namespace rsc::subprocess;

TEST(WindowsSubprocessTest, testSubprocess)
{

	// TODO this test is really a stub. Think about how to better test this on windows
	vector<string> args;
	args.push_back("--version");
    WindowsSubprocess proc(CMAKE_EXECUTABLE, args);
    boost::this_thread::sleep(boost::posix_time::seconds(5));

}
