/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2014 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include "rsc/os/ProcessInfo.h"

TEST(ProcessInfoTest, testCurrentProcessId)
{
    EXPECT_LE(0, rsc::os::currentProcessId());
}

TEST(ProcessInfoTest, testGetProgramName)
{
    EXPECT_FALSE(rsc::os::getProgramName(rsc::os::currentProcessId()).empty());
}

TEST(ProcessInfoTest, testCurrentProgramName)
{
    EXPECT_FALSE(rsc::os::currentProgramName().empty());
}

TEST(ProcessInfoTest, testGetExecutablePath)
{
    rsc::os::getExecutablePath(rsc::os::currentProcessId());
}

TEST(ProcessInfoTest, testCurrentExecutablePath)
{
    rsc::os::currentExecutablePath();
}


TEST(ProcessInfoTest, testGetCommandlineArguments)
{
    rsc::os::getCommandlineArguments(rsc::os::currentProcessId());
    std::vector<std::string> arguments
        = rsc::os::getCommandlineArguments(rsc::os::currentProcessId());
    if (!arguments.empty()) {
        if (!arguments[0].empty()) {
            EXPECT_NE(arguments[0][0], '\0');
        }
    }
}

TEST(ProcessInfoTest, testCurrentCommandlineArguments)
{
    rsc::os::currentCommandlineArguments();
}

TEST(ProcessInfoTest, testGetStartTime)
{
    rsc::os::getProcessStartTime(rsc::os::currentProcessId());
}

TEST(ProcessInfoTest, testCurrentStartTime)
{
    rsc::os::currentProcessStartTime();
}

TEST(ProcessInfoTest, testCurrentExecutingUser) {
    rsc::os::currentExecutingUser();
}
