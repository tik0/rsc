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

#include <rsc/plugins/Provider.h>

using namespace std;

// the required definitions for a plugin, built as a separate shared library
extern "C" {

// called on initialization of the plugin
RSC_PLUGIN_EXPORT void RSC_PLUGIN_INIT_SYMBOL() {
    // do your important work to make the plugin functionality known here
    cout << "My plugin is initialized!" << endl;
}

// called when unloading the plugin
RSC_PLUGIN_EXPORT void RSC_PLUGIN_SHUTDOWN_SYMBOL() {
    // remove everything your plugin provides here
    cout << "My plugin is shut down!" << endl;
}

}
