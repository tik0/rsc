/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011, 2012 Jan Moringen
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

#include <boost/filesystem/path.hpp>

#include <rsc/logging/Logger.h>

#include "ConfigSource.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/**
 * Return the directory in which system-wide configuration files are
 * located.
 *
 * @return A @ref boost::filesystem::path object representing the
 *         system configuration directory.
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path systemConfigDirectory();

/**
 * Return the home directory of the user associated with the current
 * process.
 *
 * @return A @ref boost::filesystem::path object representing the home
 *         directory of the user.
 * @throw std::runtime_error if no home directory for the user is available
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path userHomeDirectory();

/**
 * Return the canonical directory for configuration files of the user
 * associated with the current process.
 *
 * @return A @ref boost::filesystem::path object representing the
 *         configuration directory of the user.
 * @throw std::runtime_error if no config directory for the user is available
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path userConfigDirectory();


/**
 * Objects of this class analyze the environment of the current
 * process, finding environment variables whose name starts with a
 * specified string. The prefix is stripped from matching names and
 * the variables are converted to options and passed to the
 * @ref OptionHandler.
 *
 * @author jmoringe
 */
class RSC_EXPORT EnvironmentVariableSource : public ConfigSource {
public:
    /**
     * Construct a source that collect environment variables whose
     * name starts with @a prefix.
     *
     * @param prefix A prefix string against which all environment
     *               variables are matched.
     */
    EnvironmentVariableSource(const std::string& prefix = "");

    void provideOptions(OptionHandler& handler);
private:
    rsc::logging::LoggerPtr logger;

    const std::string prefix;
};

}
}
