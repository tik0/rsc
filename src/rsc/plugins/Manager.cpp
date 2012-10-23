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

#include "Manager.h"

#include <stdexcept>
#include <algorithm>

#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>

#include "../runtime/NoSuchObject.h"
#include "../logging/LoggerFactory.h"

using namespace std;

using namespace boost;
using namespace boost::filesystem;

namespace rsc {
namespace plugins {

Manager::Manager()
  : logger(logging::LoggerFactory::getInstance()
           .getLogger("rsc.plugins.Manager")) {
}

Manager::~Manager() {
}

vector<path> Manager::getPath() const {
  return this->path;
}

void Manager::addPath(const boost::filesystem::path& path) {
    // Ignore duplicates.
    if (find(this->path.begin(), this->path.end(), path)
        != this->path.end()) {
        return;
    }

    // Search specified directory for plugins.
    RSCINFO(this->logger, "Adding path `" << path << "'");
    if (is_directory(path)) {
        for (directory_iterator it = directory_iterator(path);
             it != directory_iterator(); ++it) {
            if (!is_regular_file(it->path())) {
                continue;
            }

            // Extract the plugin name from the library name.
            string name = it->path()
#if BOOST_FILESYSTEM_VERSION == 3
                .stem().string();
#else
                .stem();
#endif
            // Strip leading "lib"
            if (name.substr(0, 3) == "lib") {
                name = name.substr(3);
            }
            // Strip trailing ".so*"
            for (unsigned int i = 0; i <= 2; ++i) {
                if (name.substr(name.size() - (3 + i)).substr(0, 3)
                    == ".so") {
                    name = name.substr(0, name.size() - (3 + i));
                    break;
                }
            }
            boost::filesystem::path library = it->path();

            // If there is not yet an entry for the given name, add a
            // new plugin entry. Otherwise, ignore the plugin. This
            // logic implements precedence of searchpath entries.
            RSCINFO(this->logger, "Found plugin `"
                    << name << "' [" << library << "]");
            if (this->plugins.find(name) == this->plugins.end()) {
                this->plugins[name]
                    = Plugin::create(name, library.string());
            }
        }
    } else {
        RSCINFO(this->logger, "Ignoring non-existent path `" << path << "'");
    }
    this->path.push_back(path);
}

set<PluginPtr> Manager::getPlugins(const boost::regex& regex) const {
    set<PluginPtr> result;

    for (PluginMap::const_iterator it = this->plugins.begin();
         it != this->plugins.end(); ++it) {
        if (regex_match(it->first, regex)) {
            result.insert(it->second);
        }
    }
    return result;
  }

set<PluginPtr> Manager::getPlugins(const string& regex) const {
    return getPlugins(boost::regex(regex));
}

PluginPtr Manager::getPlugin(const string& name) const {
    PluginMap::const_iterator it;
    if ((it = this->plugins.find(name)) == this->plugins.end()) {
      throw runtime::NoSuchObject(name);
    }
    return it->second;
}

}
}
