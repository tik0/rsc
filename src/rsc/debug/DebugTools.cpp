/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "DebugTools.h"

using namespace std;

namespace rsc {
namespace debug {

string formatBacktrace(const vector<string>& trace) {
    stringstream s;
    for (std::vector<std::string>::const_iterator traceIt = trace.begin(); 
         traceIt != trace.end(); ++traceIt) {
        s << "\t" << *traceIt << std::endl;
    }
    return s.str();
}

string formatBacktrace(const unsigned int maxElements) {
    return formatBacktrace(createBacktrace(maxElements));
}

}
}
