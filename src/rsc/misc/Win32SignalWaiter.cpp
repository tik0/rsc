/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2014 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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

#include "SignalWaiter.h"

#include <Windows.h>

#include <stdexcept>

#include <iostream>

namespace rsc {
namespace misc {

int requestedSignals = 0;

void initSignalWaiter(int signals) {
    // SIGNALS should specify one or more signals.
    if (((INTERRUPT_REQUESTED | TERMINATE_REQUESTED | QUIT_REQUESTED)
         & signals) == 0) {
        throw std::logic_error("At least one signal has to be specified.");
    }

    requestedSignals = signals;
}

Signal waitForSignal() {
    if (requestedSignals == 0) {
        throw std::logic_error("initSignalWaiter has to be called before"
                               " waitForSignal.");
    }

    std::cerr << "Waiting for termination signals is not implemented for "
                 "this platform. The program will still terminate when "
                 "such a signal is received but without shutting down "
                 "properly."
              << std::endl;
    Sleep(INFINITE);
    return INTERRUPT_REQUESTED; // should be unreachable
}

bool hasSignalArrived() {
    if (requestedSignals == 0) {
        throw std::logic_error("initSignalWaiter has to be called before"
                               " hasSignalArrived.");
    }

    return false;
}


int suggestedExitCode(Signal signal) {
    switch (signal) {
    case INTERRUPT_REQUESTED:
        return 1;
    case TERMINATE_REQUESTED:
        return 1;
    case QUIT_REQUESTED:
        return 1;
    }
    throw std::logic_error("Invalid signal number");
}

}
}
