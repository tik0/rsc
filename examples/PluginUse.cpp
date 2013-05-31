/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2013 Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <iostream>

#include <stdlib.h>

#include <rsc/plugins/Manager.h>
#include <rsc/plugins/Plugin.h>

using namespace std;
using namespace rsc::plugins;

int main(int argc, char **argv) {

    if (argc <= 1) {
        cerr << "Please specify plugin paths on the command line" << endl;
        return EXIT_FAILURE;
    }

    // get the plugin manager instance for working with plugins
    Manager& pluginManager = Manager::getInstance();

    // register the user-specified plugin search paths
    for (int i = 1; i < argc; ++i) {
        pluginManager.addPath(argv[i]);
    }

    // list all available plugins that the manager can find in the search paths
    set<PluginPtr> plugins = pluginManager.getPlugins();
    cout << "The search path contains " << plugins.size() << " plugins:"
            << endl;
    for (set<PluginPtr>::iterator pluginIt = plugins.begin();
            pluginIt != plugins.end(); ++pluginIt) {
        PluginPtr plugin = *pluginIt;
        cout << "  " << plugin->getName() << endl;
    }

    // try to load and unload all found plugins
    cout << "Trying to load plugins:" << endl;
    for (set<PluginPtr>::iterator pluginIt = plugins.begin();
            pluginIt != plugins.end(); ++pluginIt) {
        PluginPtr plugin = *pluginIt;
        cout << "  " << plugin->getName() << endl;
        try {
            plugin->load();
            plugin->unload();
        } catch (const std::exception& e) {
            cerr << "  !Unable to load this plugin: " << e.what() << endl;
        } catch (...) {
            cerr << "  !Unable to load this plugin: unknown reason" << endl;
        }
    }

}

