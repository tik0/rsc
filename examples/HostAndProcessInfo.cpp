/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2014 Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <rsc/os/HostInfo.h>
#include <rsc/os/ProcessInfo.h>
#include <rsc/runtime/ContainerIO.h>

#include <boost/date_time.hpp>

using namespace std;

using namespace rsc::os;

int main(int, char*[]) {

    // Host information
    try {
        cout << "current host name: " << currentHostname() << endl;
    } catch (const runtime_error& e) {
        cout << "current host name: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "current host id: " << currentHostId() << endl;
    } catch (const runtime_error& e) {
        cout << "current host id: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "current boot time (UTC): " << currentBootTime() << endl;
    } catch (const runtime_error& e) {
        cout << "current boot time (UTC): <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }

    cout << endl;

    // Process information
    try {
        cout << "current process id: " << currentProcessId() << endl;
    } catch (const runtime_error& e) {
        cout << "current process id: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "current program name: " << currentProgramName() << endl;
    } catch (const runtime_error& e) {
        cout << "current program name: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "current executable path: " << currentExecutablePath() << endl;
    } catch (const runtime_error& e) {
        cout << "current executable path: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "current command line arguments: "
                << currentCommandlineArguments() << endl;
    } catch (const runtime_error& e) {
        cout << "current command line arguments: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "current process start time (UTC): "
                << currentProcessStartTime() << endl;
    } catch (const runtime_error& e) {
        cout << "current process start time (UTC): <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }

    return EXIT_SUCCESS;
}
