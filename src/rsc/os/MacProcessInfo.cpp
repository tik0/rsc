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

#include "ProcessInfo.h"

#include <sys/sysctl.h>
#include <sys/types.h> // for getpwuid(3)
#include <pwd.h>       // likewise
#include <unistd.h>    // for getuid(2)
#include <stdlib.h>
#include <string.h>
#include <libproc.h>

#include <boost/shared_array.hpp>

#include <boost/format.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "HostInfo.h"

namespace rsc {
namespace os{

PID currentProcessId() {
    return getpid(); // cannot fail according to getpid(2)
}

// taken from https://gist.github.com/nonowarn/770696
std::vector<std::string> getCommandlineComponents(PID pid) {

    int mib[3];
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;
    int argmax;
    size_t size = sizeof(argmax);
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " command line:"
                                                          " %1%")
                                 % strerror(errno)));
    }

    /*
     * Make a sysctl() call to get the raw argument space of the process.
     * The layout is documented in start.s, which is part of the Csu
     * project.  In summary, it looks like:
     *
     * /---------------\ 0x00000000
     * :               :
     * :               :
     * |---------------|
     * | argc          |
     * |---------------|
     * | arg[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | arg[argc - 1] |
     * |---------------|
     * | 0             |
     * |---------------|
     * | env[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | env[n]        |
     * |---------------|
     * | 0             |
     * |---------------| <-- Beginning of data returned by sysctl() is here.
     * | argc          |
     * |---------------|
     * | exec_path     |
     * |:::::::::::::::|
     * |               |
     * | String area.  |
     * |               |
     * |---------------| <-- Top of stack.
     * :               :
     * :               :
     * \---------------/ 0xffffffff
     */
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = pid;
    size = (size_t)argmax;
    boost::shared_array<char> procargs(new char[argmax]);
    if (sysctl(mib, 3, procargs.get(), &size, NULL, 0) == -1) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " command line:"
                                                          " %1%")
                                 % strerror(errno)));
    }

    // get the number of arguments from the first memory segment
    int nargs;
    memcpy(&nargs, procargs.get(), sizeof(nargs));

    // increment pointer to the memory by the size of argc to point to the first argument
    char* cp;
    cp = procargs.get() + sizeof(nargs);

    // Skip the saved exec_path
    for (; cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            // End of exec_path reached
            break;
        }
    }
    if (cp == &procargs[size]) {
        throw new std::runtime_error("Could not determine command line:"
                                     " End of info memory reached without"
                                     " finding the string area.");
    }

    // Skip trailing '\0' characters
    for (; cp < &procargs[size]; cp++) {
        if (*cp != '\0') {
          // Beginning of first argument reached
          break;
        }
    }
    if (cp == &procargs[size]) {
        throw new std::runtime_error("Could not determine command line:"
                                     " End of info memory reached without"
                                     " finding the string area.");
    }
    // cp has reached the beginning of argv[0]

    // scan through the memory starting at the begining of argv[0] and
    // grab as many null-terminated arguments as possible.
    std::vector<std::string> commandLine;
    for (int i = 0; i < nargs && cp < &procargs[size]; ++i) {
        std::string argument(cp);
        commandLine.push_back(argument);
        cp += commandLine.back().size() + 1;
    }

    return commandLine;
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
    char pathBuffer[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(pid, pathBuffer, sizeof(pathBuffer)) <= 0) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " program name:"
                                                          " %1%")
                                 % strerror(errno)));

    }
    return std::string(pathBuffer);
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

// taken from http://compgroups.net/comp.unix.programmer/how-to-get-process-start-time-in-mac-osx/2994303
boost::posix_time::ptime getProcessStartTime(PID pid) {
    int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, pid };
    struct kinfo_proc kp;
    size_t len = (size_t) sizeof(kp);
    if (sysctl(mib, 4, &kp, &len, NULL, 0) != 0) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " process start time:"
                                                          " %1%")
                                 % strerror(errno)));
    }
    if (len == 0) {
        throw std::runtime_error("Could not determine process start time:"
                                 " No such PID.");
    }

    return boost::posix_time::from_time_t(kp.kp_proc.p_starttime.tv_sec) +
        boost::posix_time::microseconds(kp.kp_proc.p_starttime.tv_usec);
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
