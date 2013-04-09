/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2012, 2013 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include "Configuration.h"

#include <stdexcept>

#include <boost/format.hpp>
#include <boost/filesystem/fstream.hpp>

#include "../logging/Logger.h"
#include "../logging/LoggerFactory.h"

#include "Environment.h"
#include "ConfigFileSource.h"
#include "CommandLinePropertySource.h"

using namespace std;

using namespace rsc::logging;

namespace rsc {
namespace config {

LoggerPtr getLogger() {
    static LoggerPtr logger
    = logging::LoggerFactory::getInstance().getLogger("rsc.config.configure()");
    return logger;
}

void configure(OptionHandler& handler,
               const string&  configFileName,
               const string&  environmentVariablePrefix,
               int            argc,
               const char**   argv,
               bool           stripEnvironmentVariablePrefix) {

    // 1) Try system-wide configuration file.
    //    (lowest priority)
    try {
        boost::filesystem::ifstream
            stream(systemConfigDirectory() / configFileName);
        if (stream) {
            ConfigFileSource source(stream);
            source.provideOptions(handler);
        }
    } catch (const runtime_error& e) {
        RSCWARN(getLogger(),
                "Failed to process system-wide configuration file `"
                << (systemConfigDirectory() / configFileName) << "': "
                << e.what());
    }

    // 2) Try user configuration file.
    bool isUserConfigDirOK = false;
    try {
        boost::filesystem::ifstream
            stream(userConfigDirectory() / configFileName);
        isUserConfigDirOK = true;
        if (stream) {
            ConfigFileSource source(stream);
            source.provideOptions(handler);
        }
    } catch (const runtime_error& e) {
        RSCWARN(getLogger(),
                "Failed to process user-specific configuration file `"
                << (isUserConfigDirOK
                    ? (userConfigDirectory() / configFileName).string()
                    : "<failed to determine user config dir>")
                << "': " << e.what());
    }

    // 3) Try configuration file in current directory.
    {
        boost::filesystem::ifstream stream(configFileName);
        if (stream) {
            ConfigFileSource source(stream);
            source.provideOptions(handler);
        }
    }

    // 4) Add environment Variables
    //    (highest priority)
    {
        EnvironmentVariableSource source(environmentVariablePrefix,
                stripEnvironmentVariablePrefix);
        source.provideOptions(handler);
    }

    // 5) Command line
    if (argc > 0) {
        CommandLinePropertySource source(argc, argv);
        source.provideOptions(handler);
    }

}

}
}
