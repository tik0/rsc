/* ============================================================
 *
 * This file is a part of the rsc project.
 *
 * Copyright (C) 2013 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "CollectingOptionHandler.h"

using namespace std;

namespace rsc {
namespace config {

CollectingOptionHandler::CollectingOptionHandler() {
}

CollectingOptionHandler::CollectingOptionHandler(
        const rsc::runtime::Properties& properties) :
        options(properties) {
}

void CollectingOptionHandler::handleOption(const vector<string>& key,
        const string& value) {
    string name;
    for (vector<string>::const_iterator it = key.begin(); it != key.end();
            ++it) {
        if (!(it == key.begin())) {
            name += ".";
        }
        name += *it;
    }
    this->options[name] = value;
}

runtime::Properties CollectingOptionHandler::getOptions() const {
    return options;
}

}
}
