/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "LinuxDebugTools.h"

#include <execinfo.h>

using namespace std;

namespace rsc {
namespace debug {

LinuxDebugTools::LinuxDebugTools() {
}

LinuxDebugTools::~LinuxDebugTools() {
}

vector<string> LinuxDebugTools::createBacktrace(const unsigned int& maxElements) {

    void** arr = (void**) malloc(maxElements * sizeof(void*));
    int nSize = backtrace(arr, maxElements);
    char** sym = backtrace_symbols(arr, nSize);

    vector<string> result;
    for (int i = 0; i < nSize; i++) {
        result.push_back(sym[i]);
    }

    free(sym);
    free(arr);

    return result;

}

}
}
