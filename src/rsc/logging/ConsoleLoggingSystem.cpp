/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of of the
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

#include "ConsoleLoggingSystem.h"

#include "ConsoleLogger.h"
#include "../misc/Registry.h"

namespace rsc {
namespace logging {

std::string ConsoleLoggingSystem::getLoggerName() {
    return "ConsoleLoggingSystem";
}

ConsoleLoggingSystem::ConsoleLoggingSystem() {
}

ConsoleLoggingSystem::~ConsoleLoggingSystem() {
}

std::string ConsoleLoggingSystem::getRegistryKey() const {
    return getLoggerName();
}

LoggerPtr ConsoleLoggingSystem::createLogger(const std::string& name) {
    return LoggerPtr(new ConsoleLogger(name));
}

CREATE_GLOBAL_REGISTREE_MSG(loggingSystemRegistry(), new ConsoleLoggingSystem, ConsoleLoggingSystem, "Could it be that you have linked two different versions of RSC in your program?")
;

}
}
