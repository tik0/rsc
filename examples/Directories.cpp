/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2013 Jan Moringen
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

#include <iostream>

#include <rsc/config.h>
#include <rsc/Version.h>
#include <rsc/config/Environment.h>

using namespace std;

using namespace rsc::config;

int main(int, char*[]) {

    cout << "RSC Version: " << rsc::Version::string()
         << " (" << rsc::Version::buildString() << ")"
         << endl;

    cout << "Install prefix:          " << RSC_INSTALL_PREFIX << endl;
    cout << "System config directory: " << systemConfigDirectory() << endl;
    cout << "Prefix config directory: " << prefixConfigDirectory() << endl;
    cout << "User home directory:     " << userHomeDirectory() << endl;
    cout << "User config directory:   " << userConfigDirectory() << endl;

    return EXIT_SUCCESS;
}
