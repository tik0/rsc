/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2014 Jan Moringen
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

#include "ProcessInfo.h"

#include <sys/types.h> // for getpwuid(3)
#include <pwd.h>       // likewise
#include <unistd.h>    // for getuid(2)

#include <boost/date_time/posix_time/posix_time.hpp>

namespace rsc {
namespace os {

unsigned int currentProcessId() {
    return getpid(); // cannot fail according to getpid(2)
}

std::string getProgramName(PID /*pid*/) {
    throw std::runtime_error("Could not determine program name: not"
                             " supported");
}

std::string currentProgramName() {
    return getProgramName(currentProcessId());
}

std::vector<std::string> getCommandlineArguments(PID /*pid*/) {
    throw std::runtime_error("Could not determine commandline arguments:"
                             " not supported");
}

std::vector<std::string> currentCommandlineArguments() {
    return getCommandlineArguments(currentProcessId());
}

std::string getExecutablePath(PID /*pid*/) {
    throw std::runtime_error("Could not determine executable path:"
                             " not supported");
}

std::string currentExecutablePath() {
    return getExecutablePath(currentProcessId());
}

boost::posix_time::ptime getProcessStartTime(PID /*pid*/) {
    throw std::runtime_error("Could not determine process start time:"
                             " not supported");
}

boost::posix_time::ptime currentProcessStartTime() {
    return getProcessStartTime(currentProcessId());
}

std::string getExecutingUser(PID /*pid*/) {
    throw std::runtime_error("Could not determine executing user:"
                             " not supported");
}

std::string currentExecutingUser() {
    passwd* entry = getpwuid(getuid());
    return entry->pw_name;
}

}
}
