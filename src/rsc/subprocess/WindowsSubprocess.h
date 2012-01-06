/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#pragma once

#include <string>
#include <vector>

#include <windows.h>

#include "Subprocess.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace subprocess {

/**
 * Subprocess implementation for windows.
 *
 * @author jwienke
 */
class RSC_EXPORT WindowsSubprocess: public Subprocess {
public:

    WindowsSubprocess(const std::string& command,
            const std::vector<std::string>& args = std::vector<std::string>());
    virtual ~WindowsSubprocess();

private:

    enum TerminateResult {
        FAILED, SUCCESS_CLEAN, SUCCESS_KILL
    };

    typedef struct {
        DWORD id;
        DWORD thread;
    } TermInfo;

    TerminateResult terminateApp(DWORD pid, DWORD timeoutMs);

    PROCESS_INFORMATION processInformation;

};

}
}

