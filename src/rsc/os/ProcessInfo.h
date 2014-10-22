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

#pragma once

#include <string>
#include <vector>

#include <boost/date_time/posix_time/ptime.hpp>

#include "rsc/rscexports.h"

namespace rsc {
namespace os {

/**
 * @defgroup os_process_info Process Information Functions
 * @{
 *
 * These functions are not guaranteed to be either thread-safe or
 * reentrant.
 */

typedef unsigned int PID;

/**
 * Return the id the current process.
 *
 * @return The id of the current process.
 *
 * @throw std::runtime_error If the process id cannot be determined.
 */
RSC_EXPORT PID currentProcessId();

/**
 * Return the name of the program executed in the process designated
 * by @a pid.
 *
 * @param pid The id of the process for which the program name should
 *            be returned.
 *
 * @return Name of the program executed in the process with the given @a pid.
 *
 * @throw std::runtime_error If the program name cannot be determined.
 */
RSC_EXPORT std::string getProgramName(PID pid);

/**
 * Return the name of the program executed in the current process.
 *
 * @return Name of the program executed in the current process.
 *
 * @throw std::runtime_error If the program name cannot be determined.
 */
RSC_EXPORT std::string currentProgramName();

/**
 * Return the absolute path of the executable file that is executed in
 * the process designated by @a pid.
 *
 * @param pid The id of the process for which the program name should
 *            be returned.
 *
 * @return The absolute path of the executable.
 *
 * @throw std::runtime_error If the executable path cannot be
 *                           determined.
 */
RSC_EXPORT std::string getExecutablePath(PID pid);

/**
 * Return the absolute path of the executable file that is executed in
 * the current process.
 *
 * @return The absolute path of the executable.
 *
 * @throw std::runtime_error If the executable path cannot be
 *                           determined.
 */
RSC_EXPORT std::string currentExecutablePath();

/**
 * Return the list of commandline arguments of the process designated
 * by @a pid.
 *
 * @param pid The id of the process for which the program name should
 *            be returned.
 *
 * @return @ref std::vector containing the commandline arguments of
 *         the process with the given @a pid.
 *
 * @throw std::runtime_error If the commandline arguments cannot be
 *                           determined.
 */
RSC_EXPORT std::vector<std::string> getCommandlineArguments(PID pid);

/**
 * Return the list of commandline arguments of the current process.
 *
 * @return @ref std::vector containing the commandline arguments of
 *         the current process.
 *
 * @throw std::runtime_error If the commandline arguments cannot be
 *                           determined.
 */
RSC_EXPORT std::vector<std::string> currentCommandlineArguments();

/**
 * Return the start time of the process designated by @a pid.
 *
 * The precision with which the start time can be determined varies
 * between platforms.
 *
 * @param pid The id of the process for which the start time should
 *            be returned.
 *
 * @return @ref boost::posix_time::ptime encoding the start time of
 *              the process.
 *
 * @throw std::runtime_error If determining the start time fails.
 */
RSC_EXPORT boost::posix_time::ptime getProcessStartTime(PID pid);

/**
 * Return the start time of the current process.
 *
 * The precision with which the start time can be determined varies
 * between platforms.
 *
 * @return @ref boost::posix_time::ptime encoding the start time of
 *              the process.
 *
 * @throw std::runtime_error If determining the start time fails.
 */
RSC_EXPORT boost::posix_time::ptime currentProcessStartTime();

/**
 * @}
 */

}
}
