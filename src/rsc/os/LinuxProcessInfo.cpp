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

#include <unistd.h>
#include <linux/param.h> // for HZ in start-time calculation

#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "HostInfo.h"

namespace rsc {
namespace os{

std::string procFilename(PID pid, const std::string& filename) {
    return boost::str(boost::format("/proc/%1%/%2%")
                      % pid % filename);
}

PID currentProcessId() {
    return getpid(); // cannot fail according to getpid(2)
}

std::vector<std::string> getCommandlineComponents(PID pid) {
    std::ifstream self(procFilename(pid, "cmdline").c_str());
    self.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    std::string cmdline;
    self >> cmdline;

    std::vector<std::string> components;
    std::string::iterator it = cmdline.begin();
    while (it != cmdline.end() && (it + 1) != cmdline.end()) {
        std::string::iterator end = find(it + 1, cmdline.end(), '\0');
        std::string component;
        std::copy(it, end, std::back_inserter(component));
        components.push_back(component);
        it = end;
    }
    return components;
}

std::string getProgramName(PID pid) {
    std::vector<std::string> components;
    try {
        components = getCommandlineComponents(pid);
    } catch (const std::exception& e) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " program name: %1%")
                                            % e.what()));
    }
    if (components.empty()) {
        throw std::runtime_error("Could not determine program name: empty"
                                 " commandline entry");
    }
    return components[0];
}

std::string currentProgramName() {
    return getProgramName(currentProcessId());
}

std::string getExecutablePath(PID pid) {
    // Try to read the target of the link /proc/PID/exe. This can fail
    // if the main thread of the process has stopped. Fall back to
    // using the cmdline pseudo-file for PID then.
    char buffer[PATH_MAX];
    int count = readlink(procFilename(pid, "exe").c_str(),
                         buffer, sizeof(buffer));
    if (count >= 0) {
        buffer[count] = '\0';
        return std::string(buffer);
    } else {
        return getProgramName(pid);
    }
}

std::string currentExecutablePath() {
    return getExecutablePath(currentProcessId());
}

std::vector<std::string> getCommandlineArguments(PID pid) {
    std::vector<std::string> result;
    try {
        result = getCommandlineComponents(pid);
    } catch (const std::exception& e) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " commandline arguments:"
                                                          " %1%")
                                % e.what()));
    }
    if (result.empty()) {
        throw std::runtime_error("Could not determine commandline arguments:"
                                 " empty commandline entry");
    }

    result.erase(result.begin());
    return result;
}

std::vector<std::string> currentCommandlineArguments() {
    return getCommandlineArguments(currentProcessId());
}

// FIXME this has a critical flaw: the HZ macro is obviously a
// compile-time constant referring to the system timer interval of the
// compilation host system. However, when a compiled binary is
// executed on a different system, the system timer interval of that
// system may be different. Unfortunately, there does not seem to be a
// means to determine the value a runtime.
boost::posix_time::ptime getProcessStartTime(PID pid) {
    static const unsigned int START_TIME_BOOT_JIFFIES_FIELD_NUMBER = 22;

    // Read process start time in jiffies since *system boot*.
    // See /proc/[pid]/stat section in proc(5).
    boost::uint64_t startTimeBootJiffies;
    {
        const std::string procSelfStat = procFilename(pid, "stat");
        std::ifstream stream(procSelfStat.c_str());
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));
        if (tokens.size() < START_TIME_BOOT_JIFFIES_FIELD_NUMBER) {
            throw std::runtime_error(boost::str(boost::format("%1% did not contain"
                                                              " the expected number"
                                                              " of fields")
                                                % procSelfStat));
        }
        try {
            startTimeBootJiffies
                = boost::lexical_cast<boost::uint64_t>
                (tokens[START_TIME_BOOT_JIFFIES_FIELD_NUMBER - 1]);
        } catch (const boost::bad_lexical_cast& e) {
            throw std::runtime_error(boost::str(boost::format("Could not"
                                                              "parse process"
                                                              " start-time entry"
                                                              " of %1%: %2%")
                                                % procSelfStat % e.what()));
        }
    }

    // Add to system boot time the process start time relative to
    // system boot time. Process integral seconds and milliseconds.
    boost::posix_time::ptime bootTime = currentBootTime();
    return boost::posix_time::ptime
        (bootTime.date(),
         bootTime.time_of_day()
         + boost::posix_time::seconds(startTimeBootJiffies / HZ)
         + boost::posix_time::milliseconds((1000/HZ * startTimeBootJiffies)
                                           % 1000));
}

boost::posix_time::ptime currentProcessStartTime() {
    return getProcessStartTime(currentProcessId());
}

}
}
