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

#include <boost/date_time/posix_time/ptime.hpp>

#include "rsc/rscexports.h"

namespace rsc {
namespace os {

/**
 * @defgroup os_host_info Host Information Functions
 * @{
 *
 * These functions are not guaranteed to be either thread-safe or
 * reentrant.
 */

/**
 * Determine and return the machine type, usually CPU architecture, of
 * the local machine.
 *
 * @return The machine type of the local machine.
 *
 * @throw std::runtime_error If the machine type cannot be determined.
 */
RSC_EXPORT std::string currentMachineType();

/**
 * Determine and return the version within its type, usually the CPU
 * identification string, of the local machine.
 *
 * @return The machine version of the local machine.
 *
 * @throw std::runtime_error If the machine version cannot be
 *                           determined.
 */
RSC_EXPORT std::string currentMachineVersion();

/**
 * Determine and return the type of the operating system, usually the
 * kernel name, running on the local machine.
 *
 * @return The software type of the local machine.
 *
 * @throw std::runtime_error If the software type cannot be
 *                           determined.
 */
RSC_EXPORT std::string currentSoftwareType();

/**
 * Determine and return the version of the operating system within its
 * type, usually the kernel version string, running on the local
 * machine.
 *
 * @return The software version of the local machine.
 *
 * @throw std::runtime_error If the software version cannot be
 *                           determined.
 */
RSC_EXPORT std::string currentSoftwareVersion();

/**
 * Determine and return the hostname of the local machine.
 *
 * Note that the hostname can change over time.
 *
 * @return The current hostname of the local machine.
 *
 * @throw std::runtime_error If the hostname cannot be determined.
 */
RSC_EXPORT std::string currentHostname();

/**
 * Determine and return a unique id string of the local machine.
 *
 * This unique id should not change over time but callers should still
 * be prepared to deal with that since not all implementation can
 * provide a constant unique id.
 *
 * @return A unique id string for the local machine.
 *
 * @throw std::runtime_error If the unique id cannot be determined.
 */
RSC_EXPORT std::string currentHostId();

/**
 * Return the boot time of the local machine.
 *
 * The precision with which the boot time can be determined varies
 * between platforms.
 *
 * @return @ref boost::posix_time::ptime encoding the boot time of the
 *              machine.
 *
 * @throw std::runtime_error If determining the boot time fails.
 */
RSC_EXPORT boost::posix_time::ptime currentBootTime();

/**
 * @}
 */

}
}
