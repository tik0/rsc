/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2012, 2013, 2016 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

void describeFileStream(const std::string&             label,
                        const boost::filesystem::path& path,
                        const istream&                 stream) {
    cerr << "  " << label << " " << path << " "
         << (stream ? "exists" : "does not exist") << endl;
}

void configure(OptionHandler&                 handler,
               const string&                  configFileName,
               const string&                  environmentVariablePrefix,
               int                            argc,
               const char**                   argv,
               bool                           stripEnvironmentVariablePrefix,
               const boost::filesystem::path& prefix) {
    bool debug = getEnvironmentVariable("__CONFIG_DEBUG").get();

    // 0) In debug mode, header first.
    if (debug) {
        cerr << "Configuring with sources (lowest priority first)" << endl
             << "  1. Configuration files" << endl;
    }

    // 1) Try prefix-wide configuration file
    //    (lowest priority)
    boost::filesystem::path
        prefixWideFile(prefixConfigDirectory(prefix) / configFileName);
    try {
        boost::filesystem::ifstream stream(prefixWideFile);
        if (debug) {
            describeFileStream("   1. Prefix wide config file",
                               prefixWideFile, stream);
        }
        if (stream) {
            ConfigFileSource source(stream);
            source.provideOptions(handler);
        }
    } catch (const runtime_error& e) {
        RSCWARN(getLogger(),
                "Failed to process prefix-wide configuration file `"
                << prefixWideFile << "': " << e.what());
    }

    // 2) Try user configuration file.
    //
    // The two instance of userConfigDirectory() / configFileName in
    // the following code cannot be reduce into a variable assignment
    // outside the try/catch block since the code can throw an
    // exception.
    bool isUserConfigDirOK = false;
    try {
        boost::filesystem::path userFile(userConfigDirectory()
                                         / configFileName);
        boost::filesystem::ifstream stream(userFile);
        if (debug) {
            describeFileStream("   2. User config file", userFile, stream);
        }
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
        if (debug) {
            describeFileStream("   3. Current directory file",
                               configFileName, stream);
        }
        if (stream) {
            ConfigFileSource source(stream);
            source.provideOptions(handler);
        }
    }

    // 4) Add environment Variables
    {
        EnvironmentVariableSource source(environmentVariablePrefix,
                stripEnvironmentVariablePrefix);
        if (debug) {
            cerr << "  2. Environment variables with prefix "
                 << environmentVariablePrefix << endl;
            EnvironmentVariableSource::Matches matches = source.getMatches();
            if (!matches.empty()) {
                for (EnvironmentVariableSource::Matches::const_iterator it
                         = matches.begin(); it != matches.end(); ++it) {
                    cerr << "     "
                         << it->getRawName() << " -> " << it->getValue()
                         << endl;
                }
            } else {
                cerr << "     <none>" << endl;
            }
        }
        source.provideOptions(handler);
    }

    // 5) Command line
    //    (highest priority)
    if (debug) {
        cerr << "  3. Commandline options" << endl;
    }
    if (argc > 0) {
        if (debug){
            for (int i = 0; i < argc; ++i) {
                cerr << "     " << argv[i] << endl;
            }
        }
        CommandLinePropertySource source(argc, argv);
        source.provideOptions(handler);
    } else {
        if (debug) {
            cerr << "     <none>" << endl;
        }
    }

}

}
}
