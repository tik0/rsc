/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2016 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include <fstream>

#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <rsc/config/Environment.h>
#include <rsc/config/Configuration.h>

#include "testconfig.h"

using namespace rsc::config;

TEST(ResolveConfigurationFileTest, testSmoke) {
    EXPECT_EQ("foo.conf",
              resolveConfigurationFile("foo.conf", "/pre", "c.conf").first);

    EXPECT_EQ("/pre/etc/c.conf",
              resolveConfigurationFile(CONFIG_FILE_KEY_PREFIX, "/pre", "c.conf").first);

    EXPECT_EQ((userConfigDirectory() / "c.conf"),
              resolveConfigurationFile(CONFIG_FILE_KEY_USER, "/pre", "c.conf").first);

    EXPECT_EQ("c.conf",
              resolveConfigurationFile(CONFIG_FILE_KEY_PWD, "/pre", "c.conf").first);
}
