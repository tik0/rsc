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
#include "rsc/runtime/Demangle.h"
#include <execinfo.h>
#include <sstream>

using namespace std;

namespace rsc {
namespace debug {

// the stacktrace contains the path and a mangled function name
// <lib name>(<mangled name>+<address offset>) [<address>]
static string demangle(char* sym) {
    char *begin = 0, *end = 0;
    // find the parentheses and address offset surrounding the mangled name
    for (char *i = sym; *i; ++i) {
       if (*i == '(') begin = i;
       else if (*i == '+') end = i;
    }

    if (begin && end) {
        // found our mangled name, now in [begin, end)
        *begin++ = '\0'; // replace '('
        *end = '\0';     // replace '+'

        ::std::ostringstream oss;
        try {
            oss << sym << ": " << rsc::runtime::demangle(begin);
            return oss.str();
        } catch (const std::exception &e) {
        }
    }
    // if we come here, demangling failed: just use the whole string
    return sym;
}

vector<string> createBacktrace(const unsigned int maxElements) {

    void** arr = (void**) malloc(maxElements * sizeof(void*));
    int nSize = backtrace(arr, maxElements);
    char** sym = backtrace_symbols(arr, nSize);

    vector<string> result;
    for (int i = 0; i < nSize; i++) {
        result.push_back(demangle(sym[i]));
    }

    free(sym);
    free(arr);

    return result;

}

}
}
