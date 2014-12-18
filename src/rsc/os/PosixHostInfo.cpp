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

#include <stdexcept>
#include <sstream>

namespace rsc {
namespace os {

// Host ID

std::string currentHostId() {
    std::ostringstream stream;
    stream << std::hex << gethostid(); // cannot fail according to gethostid(3)
    return stream.str();
}

// Boot time

boost::posix_time::ptime currentBootTime() {
    throw std::runtime_error("Could not determine boot time: not supported.");
}

}
}
