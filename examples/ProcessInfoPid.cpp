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

#include <rsc/os/ProcessInfo.h>
#include <rsc/runtime/ContainerIO.h>

#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

using namespace rsc::os;

int main(int argc, char* argv[]) {

    if (argc < 2) {
       std::cerr << "Usage: " << argv[0] << " pid" << std::endl;
       return EXIT_FAILURE;
    }

    rsc::os::PID pid = boost::lexical_cast<rsc::os::PID> (argv[1]);

    // Process information
    try {
        cout << "program name: " << getProgramName(pid) << endl;
    } catch (const runtime_error& e) {
        cout << "program name: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "executable path: " << getExecutablePath(pid) << endl;
    } catch (const runtime_error& e) {
        cout << "executable path: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "command line arguments: "
                << getCommandlineArguments(pid) << endl;
    } catch (const runtime_error& e) {
        cout << "command line arguments: <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }
    try {
        cout << "process start time (UTC): "
                << getProcessStartTime(pid) << endl;
    } catch (const runtime_error& e) {
        cout << "process start time (UTC): <unknown>" << endl;
        cout << "    reason: " << e.what() << endl;
    }

    return EXIT_SUCCESS;
}
