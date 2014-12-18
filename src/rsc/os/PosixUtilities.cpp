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

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#include <stdexcept>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>

namespace rsc {
namespace os {

// {Machine,Software} {Type,Version}

std::string callWithUtsname(const std::string&                            context,
                            boost::function1<std::string, const utsname&> thunk) {
    utsname info;
    if (uname(&info) == -1) {
        throw std::runtime_error(boost::str(boost::format("Could not determine %1% because"
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
    char buffer[HOSTNAME_MAX_LENGTH];
    if (gethostname(buffer, HOSTNAME_MAX_LENGTH) == 0) {
        std::string maybeQualifiedHostname(buffer);
        std::vector<std::string> components;
        boost::algorithm::split(components, maybeQualifiedHostname,
                                boost::algorithm::is_any_of("."));
        assert(!components.empty());
        return components[0];
    } else {
        throw std::runtime_error(boost::str(boost::format("gethostname failed: %1%")
                                            % strerror(errno)));
    }
}

}
}
