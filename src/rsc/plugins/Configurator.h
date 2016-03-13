/* ============================================================
 *
 * This file is part of the RSC project.
 *
 * Copyright (C) 2012, 2014, 2016 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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
 * ============================================================  */

#pragma once

#include <string>
#include <vector>

#include "../config/OptionHandler.h"
#include "../logging/Logger.h"

#include "Manager.h"

#include <rsc/rscexports.h>

namespace rsc {
namespace plugins {

/**
 * Instances of this class can be used to configure the #Manager based
 * on configuration options.
 *
 * @author jmoringe
 */
class RSC_EXPORT Configurator : public config::OptionHandler {
public:
    /**
     * Constructs a @c Configurator with default plugin search path @a
     * defaultPath.
     *
     * @param manager the manager to configure
     * @param defaultPath A #vector of #boost::filesystem::path
     *                    objects which should be installed as plugin
     *                    search path in case no plugin search path is
     *                    configured.
     */
    Configurator(ManagerPtr manager,
                 const std::vector<boost::filesystem::path>& defaultPath);
    virtual ~Configurator();

    void handleOption(const std::vector<std::string>& key,
                      const std::string& value);

    /**
     * Performs the actual loading of plugins. Potential errors are reported via
     * exceptions here.
     *
     * @param errorOnMissing If @c true, report and error in case a requested
     *                       plugin could not be loaded.
     *
     * @throws rsc::runtime::NoSuchObject a requested plugin could not be found
     * @throws std::runtime_error A plugin failed to load
     */
    void execute(bool errorOnMissing=true);

private:
    logging::LoggerPtr logger;

    ManagerPtr manager;

    std::vector<boost::filesystem::path> defaultPath;

    std::vector<std::string>             path;
    std::vector<std::string>             load;

    void addDefaultPath();

    void addPathEntries(const std::vector<std::string>& entries);

    void loadPlugins(const std::vector<std::string>& names,
                     bool errorOnMissing);
};

}
}
