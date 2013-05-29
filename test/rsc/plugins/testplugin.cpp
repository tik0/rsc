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

#include <fstream>

#include <boost/filesystem.hpp>

#include <rsc/plugins/Provider.h>

#include "testconfig.h"

using namespace std;

extern "C" {

void RSC_PLUGIN_INIT_SYMBOL() {

    boost::filesystem::path callFilePath(PLUGIN_CALL_FILE);
    boost::filesystem::create_directories(callFilePath.parent_path());
    ofstream callFile;
    callFile.open (callFilePath.c_str(), ios::app);
    callFile << "INIT\n";
    callFile.close();

}

void RSC_PLUGIN_SHUTDOWN_SYMBOL() {

    boost::filesystem::path callFilePath(PLUGIN_CALL_FILE);
    boost::filesystem::create_directories(callFilePath.parent_path());
    ofstream callFile;
    callFile.open (callFilePath.c_str(), ios::app);
    callFile << "SHUTDOWN\n";
    callFile.close();

}

}
