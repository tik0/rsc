/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2014 Johannes Wienke <jwienke@techfak.uni-bielefeld.de>
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

#include <errno.h>
#include <string.h>

#include <signal.h>
#include <semaphore.h>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "langutils.h"

namespace rsc {
namespace misc {

int requestedSignals = 0;
static std::string semaphore_name;
sem_t* semaphore;
volatile sig_atomic_t receivedSignal = 0;

void handler(int signal) {
    // Store the number of the received signal and post a token to the
    // semaphore to allow the blocking sem_wait() call in
    // waitForSignal() to complete.
    receivedSignal = signal;
    sem_post(semaphore);
}

void throwRuntimeError(int errorNumber, const std::string& description) {
    char buffer[256];
    strerror_r(errorNumber, buffer, 256);
    throw std::runtime_error(boost::str(boost::format("%1%: %2%")
                                        % description % buffer));
}

void initSignalWaiter(int signals) {
    // SIGNALS should specify one or more signals.
    if (((INTERRUPT_REQUESTED | TERMINATE_REQUESTED | QUIT_REQUESTED)
         & signals) == 0) {
        throw std::logic_error("At least one signal has to be specified.");
    }

    requestedSignals = signals;

    // Initialize the semaphore to a blocked state.
    semaphore_name = (randAlnumStr(3)
                      + boost::lexical_cast<std::string>(currentTimeMicros()));
    if (SEM_FAILED == (semaphore = sem_open(semaphore_name.c_str(), O_CREAT | O_EXCL, 0644, 0))) {
        throwRuntimeError(errno, "Failed to initialize semaphore");
    }

    // The arrival of any of the requested signals unblocks the
    // semaphore.
    //
    // SA_RESETHAND causes the original handler to be restored after
    // completion of our handler. This reactivates the default
    // behavior of terminating the program for e.g. a second SIGINT
    // signal.
    struct sigaction action;
    action.sa_handler = &handler;
    if (sigemptyset(&action.sa_mask) != 0) {
        throwRuntimeError(errno, "Failed to adjust signal mask");
    }
    action.sa_flags = SA_RESETHAND;
    if ((INTERRUPT_REQUESTED & signals) != 0) {
        if (sigaction(SIGINT, &action, 0) != 0) {
            throwRuntimeError(errno, "Failed to install SIGINT handler");
        }
    }
    if ((TERMINATE_REQUESTED & signals) != 0) {
        if (sigaction(SIGTERM, &action, 0) != 0) {
            throwRuntimeError(errno, "Failed to install SIGTERM handler");
        }
    }
    if ((QUIT_REQUESTED & signals) != 0) {
        if (sigaction(SIGQUIT, &action, 0) != 0) {
            throwRuntimeError(errno, "Failed to install SIGQUIT handler");
        }
    }
}

Signal mappedSignal() {

    if (receivedSignal == 0) {
        return NO_SIGNAL;
    } else if (receivedSignal == SIGINT) {
        return INTERRUPT_REQUESTED;
    } else if (receivedSignal == SIGTERM) {
        return TERMINATE_REQUESTED;
    } else if (receivedSignal == SIGQUIT) {
        return QUIT_REQUESTED;
    } else {
        throw std::runtime_error(
                boost::str(
                        boost::format("unexpected signal number %1%")
                                % (int) receivedSignal));
    }

}

Signal waitForSignal() {
    if (requestedSignals == 0) {
        throw std::logic_error("initSignalWaiter has to be called before"
                               " waitForSignal.");
    }

    // sem_wait can be interrupted by any signal. Since we only want
    // to wait for the specified signals, we have to resume waiting in
    // case of random unrelated interruptions.
    int result;
    while ((-1 == (result = sem_wait(semaphore))) && (errno == EINTR));
    if (result == -1) {
        throwRuntimeError(errno, "Failed to wait for signal");
    }

    // Even though potentially two different signals could be handled
    // in parallel, we need to destroy the semaphore on mac, living
    // with the potential crash situation, in order to not fill up the
    // persistent resources for named semaphores.
    // TODO restore all handlers to default action to prevent this?
    if (sem_unlink(semaphore_name.c_str()) == -1) {
        throwRuntimeError(errno, "Could not unlink semaphore used for waiting");
    }
    if (sem_close(semaphore) == -1) {
        throwRuntimeError(errno, "Could not close semaphore used for waiting");
    }

    assert(receivedSignal != 0);
    return mappedSignal();
}

Signal lastArrivedSignal() {
    if (requestedSignals == 0) {
        throw std::logic_error("initSignalWaiter has to be called before"
                               " hasSignalArrived.");
    }

    return mappedSignal();
}

int suggestedExitCode(Signal signal) {
    switch (signal) {
    case INTERRUPT_REQUESTED:
        return 1;
    case TERMINATE_REQUESTED:
        return 1;
    case QUIT_REQUESTED:
        return 1;
    case NO_SIGNAL:
    default:
        throw std::logic_error("Invalid signal number");
    }
}

}
}
