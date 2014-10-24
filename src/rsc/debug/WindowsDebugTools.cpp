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
#include "StackWalker.h"

using namespace std;

namespace rsc {
namespace debug {

class MyStackWalker : public StackWalker
{
public:
    MyStackWalker() : StackWalker() {}
    MyStackWalker(DWORD dwProcessId, HANDLE hProcess) : StackWalker(dwProcessId, hProcess) {}
    virtual void OnOutput(LPCSTR szText) {
        stack.push_back(szText);
        StackWalker::OnOutput(szText);
    }
    const vector<string> getStackTrace() const {return stack;}

private:
    vector<string> stack;
};

/* utilize StackWalker from:
 * http://www.codeproject.com/Articles/11132/Walking-the-callstack
 */
vector<string> createBacktrace(const unsigned int maxElements) {
    MyStackWalker sw;
    sw.ShowCallstack();
    return sw.getStackTrace();
}

}
}
