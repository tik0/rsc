/* ============================================================
 *
 * This file is part of the RSB project.
 *
 * Copyright (C) 2012 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include "Configurator.h"

#include <boost/format.hpp>

#include <boost/tokenizer.hpp>

#include "../runtime/ContainerIO.h"

#include "../logging/LoggerFactory.h"

using namespace std;

using namespace boost;
using namespace boost::filesystem;

namespace rsc {
namespace plugins {

Configurator::Configurator(ManagerPtr manager, const vector<path>& defaultPath)
    : logger(logging::LoggerFactory::getInstance()
             .getLogger("rsc.plugins.Configurator")),
      manager(manager),
      pathSet(false),
      defaultPath(defaultPath) {
}

Configurator::~Configurator() {
    // If a searchpath has not been set via configuration options, use
    // the default.
    if (!this->pathSet) {
        RSCINFO(this->logger, "No searchpath configured; using default: "
                << this->defaultPath);
        addDefaultPath();
    }

    // Load all requested plugins.
    for (set<string>::const_iterator it = this->load.begin();
         it != this->load.end(); ++it) {
        // Treat each element as a regular expression. Find matching
        // plugins and load them. Note that regex syntax error can
        // cause an exception here.
        string pattern = *it;
        RSCDEBUG(this->logger, "Looking up plugins matching pattern `"
                 << pattern << "'");
        set<PluginPtr> matches;
        try {
            matches = this->manager->getPlugins(boost::regex(pattern));
        } catch (const std::exception& e) {
            throw runtime_error(str(format("Failed to lookup matching plugins for patterns `%1%' as requested via configuration: %2%")
                                    % pattern
                                    % e.what()));
        }
        RSCDEBUG(this->logger, "Found " << matches.size() << " match(es)");

        // Try to load all plugins matching the pattern.
        for (set<PluginPtr>::iterator it = matches.begin();
             it != matches.end(); ++it) {
            RSCDEBUG(this->logger, "Loading plugin " << (*it)->getName());
            try {
                (*it)->load();
            } catch (const std::exception& e) {
                throw runtime_error(str(format("Failed to load plugin `%1%' as requested via configuration: %2%")
                                        % (*it)->getName()
                                        % e.what()));
            }
        }
    }
}

void Configurator::handleOption(const vector<string>& key,
                                const string& value) {
    RSCDEBUG(this->logger, "Processing option " << key << " = `" << value << "'");

    // Ignore other options.
    if (!((key.size() == 3)
          && (key[0] == "plugins")
          && (key[1] == "cpp"))) {
        return;
    }

    // Process plugins.cpp.{path,load} options.
    if (key[2] == "path") {
        // Split value at ":" tokens. Each substring is a searchpath
        // entry.
        vector<string> paths = splitValue(value);

        // Add all specified paths. The empty string, produced by "::"
        // means that the default searchpath should be spliced in.
        for (vector<string>::const_iterator it = paths.begin();
             it != paths.end(); ++it) {
            string path = *it;
            try {
                if (path.empty()) {
                    path = "<default path>"; // for exception message
                    addDefaultPath();
                } else {
                    this->manager->addPath(path);
                }
            } catch (const std::exception& e) {
                throw runtime_error(str(format("Failed to add path `%1%' as requested via configuration: %2%")
                                        % path
                                        % e.what()));
            }
        }

        this->pathSet = true;
    } else if (key[2] == "load") {
        vector<string> temp = splitValue(value);
        copy(temp.begin(), temp.end(), inserter(this->load, this->load.begin()));
    } else {
        throw invalid_argument(str(format("Invalid option key `%1%'; plugin related option keys are `path' and `load'.")
                                   % key));
    }
}

void Configurator::addDefaultPath() {
    for (vector<path>::const_iterator it = this->defaultPath.begin();
         it != this->defaultPath.end(); ++it) {
        this->manager->addPath(*it);
    }
}

vector<string> Configurator::splitValue(const string& value) const {
    vector<string> result;
    boost::escaped_list_separator<char> sep('\\', ':');
    boost::tokenizer< boost::escaped_list_separator<char> > tok(value, sep);
    copy(tok.begin(), tok.end(), back_inserter(result));
    return result;
}

}
}
