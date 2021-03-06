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

#include "rsc/os/HostInfo.h"

TEST(HostInfoTest, testCurrentHostId)
{
    try {
        EXPECT_FALSE(rsc::os::currentHostId().empty());
    } catch (const std::runtime_error& e) {
        // errors are expected on window. Ignore them.
    }
}

TEST(HostInfoTest, testCurrentHostname)
{
    EXPECT_FALSE(rsc::os::currentHostname().empty());
}

TEST(HostInfoTest, testCurrentMachineType)
{
    try {
        EXPECT_FALSE(rsc::os::currentMachineType().empty());
    } catch (const std::runtime_error& e) {
        // Functionality may not be available on all platforms =>
        // ignore errors.
    }
}

TEST(HostInfoTest, testCurrentMachineVersion)
{
    try {
        EXPECT_FALSE(rsc::os::currentMachineVersion().empty());
    } catch (const std::runtime_error& e) {
        // Functionality may not be available on all platforms =>
        // ignore errors.
    }
}

TEST(HostInfoTest, testCurrentSoftwareType)
{
    try {
        EXPECT_FALSE(rsc::os::currentSoftwareType().empty());
    } catch (const std::runtime_error& e) {
        // Functionality may not be available on all platforms =>
        // ignore errors.
    }
}

TEST(HostInfoTest, testCurrentSoftwareVersion)
{
    try {
        EXPECT_FALSE(rsc::os::currentSoftwareVersion().empty());
    } catch (const std::runtime_error& e) {
        // Functionality may not be available on all platforms =>
        // ignore errors.
    }
}
