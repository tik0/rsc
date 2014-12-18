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

#include "HostInfo.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include <stdexcept>
#include <sstream>

#include <boost/format.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace rsc {
namespace os {

// {Machine,Software} {Type,Version}

std::string currentMachineVersion () {
    char buffer[256];
    size_t size = 256;
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &size, 0, 0) != 0) {
        throw std::runtime_error(boost::str(boost::format("sysctlbyname failed: %1%")
                                            % strerror(errno)));
    }
    return std::string(buffer);
}

// Host ID

std::string currentHostId() {
    uuid_t id;
    struct timespec wait;
    wait.tv_sec = 0;
    wait.tv_nsec = 0;
    if (gethostuuid(id, &wait) != 0) {
        throw std::runtime_error(boost::str(boost::format("gethostuuid failed: %1%")
                                            % strerror(errno)));
    }
    std::stringstream stream;
    for (unsigned int i = 0; i < 16; ++i) {
        stream << std::hex << std::setw(2) << std::setfill('0') << (int) id[i];
    }
    return stream.str();
}

// Boot time

boost::posix_time::ptime currentBootTime() {
    int mib[] = { CTL_KERN, KERN_BOOTTIME };
    struct timeval t;
    size_t len = (size_t) sizeof(t);
    if (sysctl(mib, 2, &t, &len, NULL, 0) != 0) {
        throw std::runtime_error(boost::str(boost::format("Could not determine"
                                                          " computer boot time:"
                                                          " %1%")
                                 % strerror(errno)));
    }
    return boost::posix_time::from_time_t(t.tv_sec) +
        boost::posix_time::microseconds(t.tv_usec);
}

}
}
