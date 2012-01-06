/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
 *
 * This file may be licensed under the terms of of the
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

#include "TypedValue.h"

#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace std;

using namespace boost;

namespace rsc {
namespace config {

any parseTypedValue(const string& value) {
    if ((value[0] == '<') && (value.find('>') != string::npos)) {
        unsigned int index = value.find('>');
        string type = value.substr(1, index - 1);
        string rest = value.substr(index + 1);
        if (type == "bool") {
            return any(lexical_cast<bool>(rest));
        } else if (type == "uint") {
            return any(lexical_cast<unsigned int>(rest));
        } else if (type == "int") {
            return any(lexical_cast<int>(rest));
        } else if (type == "double") {
            return any(lexical_cast<double>(rest));
        } else {
            throw invalid_argument(str(format("Unknown option type `%1%'")
                                       % type));
        }
    } else {
        return any(value);
    }
}

}
}
