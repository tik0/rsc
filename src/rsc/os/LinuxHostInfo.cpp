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

#include "HostInfo.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#include <stdexcept>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace rsc {
namespace os {

// {Machine,Software} {Type,Version}

std::string currentMachineVersion() {
    // Read entire contents of /proc/cpuinfo, preserving whitespace.
    const std::string procCPUInfo = "/proc/cpuinfo";
    std::ifstream stream(procCPUInfo.c_str());
    stream >> std::noskipws;
    std::string content;
    std::copy(std::istream_iterator<char>(stream),
              std::istream_iterator<char>(),
              std::back_inserter(content));
    // Locate the "model name" field.
    std::string pattern("model name	: ");
    std::string::iterator start = std::search(content.begin(), content.end(),
                                              pattern.begin(), pattern.end());
    if (start == content.end()) {
        throw std::runtime_error(
            boost::str(boost::format("Could not determine machine version "
                                     "since the \"%1\" entry could not be "
                                     "found in the \"%2%\" file.")
                       % pattern % procCPUInfo));
    }
    // Field value is everything from ": " to end of line.
    std::string::iterator end = std::find(start, content.end(), '\n');
    if (end == content.end()) {
        throw std::runtime_error(
            boost::str(boost::format("Could not determine machine version "
                                     "since the end of the \"%1\" entry "
                                     "could not be found in the \"%2%\" "
                                     "file.")
                       % pattern % procCPUInfo));
    }
    std::string value;
    std::copy(start + pattern.size(), end, std::back_inserter(value));
    return value;
}

std::string callWithUtsname(const std::string&                            context,
                            boost::function1<std::string, const utsname&> thunk) {
    using namespace boost;

    utsname info;
    if (uname(&info) == -1) {
        throw std::runtime_error(str(format("Could not determine %1% because"
                                            " uname(2) failed: %2%")
                                     % context % strerror(errno)));
    }
    return thunk(info);
}

struct GetMachineType {
    std::string operator()(const utsname& info) const {
        if (std::string(info.machine) == "i686") {
            return "x86";
        } else {
            return info.machine;
        }
    }
};

std::string currentMachineType() {
    return callWithUtsname("machine type", GetMachineType());
}

struct GetSysname {
    std::string operator()(const utsname& info) const {
        std::string result(info.sysname);
        boost::algorithm::to_lower(result);
        return result;
    }
};

std::string currentSoftwareType() {
    return callWithUtsname("software type", GetSysname());
}

struct GetRelease {
    std::string operator()(const utsname& info) const {
        return info.release;
    }
};

std::string currentSoftwareVersion() {
    return callWithUtsname("software version", GetRelease());
}

// Hostname

const unsigned int HOSTNAME_MAX_LENGTH = 1024;

std::string currentHostname() {
    using namespace boost;

    char buffer[HOSTNAME_MAX_LENGTH];
    if (gethostname(buffer, HOSTNAME_MAX_LENGTH) == 0) {
        return std::string(buffer);
    } else {
        throw std::runtime_error(str(format("gethostname failed: %1%")
                                     % strerror(errno)));
    }
}

// Host ID

std::string tryFile(const std::string& filename) {
    std::string result = "";
    std::ifstream stream(filename.c_str());
    if (stream.good()) {
        stream >> result;
    }
    return result;
}

std::string hostId() {
    std::ostringstream stream;
    stream << std::hex << gethostid(); // cannot fail according to gethostid(3)
    return stream.str();
}

std::string currentHostId() {
    std::string result;
    if (!(result = tryFile("/etc/machine-id")).empty()) {
        return result;
    } else if (!(result = tryFile("/var/lib/dbus/machine-id")).empty()) {
        return result;
    } else {
        return hostId();
    }
}

boost::posix_time::ptime currentBootTime() {
    // Read system boot time in integral seconds since UNIX epoch.
    // See /proc/stat section in proc(5).
    boost::uint32_t bootTimeUNIXSeconds;

    const std::string procStat = "/proc/stat";
    std::ifstream stream(procStat.c_str());
    stream >> std::noskipws;
    std::string content;
    std::copy(std::istream_iterator<char>(stream),
              std::istream_iterator<char>() ,
              std::back_inserter(content));
    std::string pattern("btime ");
    std::string::iterator start = std::search(content.begin(), content.end(),
                                              pattern.begin(), pattern.end());
    if (start == content.end()) {
        throw std::runtime_error(
                boost::str(boost::format("Could not determine the boot time "
                                         "since the \"%1\" entry could not be "
                                         "found in the \"%2%\" file.")
                           % pattern % procStat));
    }
    std::string::iterator end = std::find(start, content.end(), '\n');
    if (end == content.end()) {
        throw std::runtime_error(
                boost::str(boost::format("Could not determine the boot time "
                                         "since the end of the \"%1\" entry "
                                         "could not be found in the \"%2%\" "
                                         "file.")
                           % pattern % procStat));
    }
    std::string value;
    std::copy(start + pattern.size(), end, std::back_inserter(value));
    try {
        bootTimeUNIXSeconds = boost::lexical_cast<boost::int32_t>(value);
    } catch (const boost::bad_lexical_cast& e) {
        throw std::runtime_error(
                boost::str(boost::format("Could not determine the boot time "
                                         "since the \"%1\" entry in the "
                                         "\"%2%\" file could not be parsed. "
                                         "Reason: %3%")
                           % pattern % procStat % e.what()));
    }

    return boost::posix_time::from_time_t(bootTimeUNIXSeconds);
}

}
}
