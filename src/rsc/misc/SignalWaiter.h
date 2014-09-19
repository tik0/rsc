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

#pragma once

#include <stdexcept>

#include "rsc/rscexports.h"

namespace rsc {
namespace misc {

/**
 * Signals @ref waitForSignal can wait for.
 *
 * The numeric values are chosen in a way that allows combining values
 * via logical "or".
 *
 * @author jmoringe
 */
enum Signal {

    /**
     * Interrupting the program has be requested.
     *
     * On POSIX platforms, this corresponds to @c SIGINT.
     */
    INTERRUPT_REQUESTED = 0x01,

    /**
     * Terminating the program has be requested.
     *
     * On POSIX platforms, this corresponds to @c SIGTERM.
     */
    TERMINATE_REQUESTED = 0x02,

    /**
     * Quitting the program has be requested.
     *
     * On POSIX platforms, this corresponds to @c SIGQUIT.
     */
    QUIT_REQUESTED      = 0x04

};

/**
 * Prepare waiting for @a signals.
 *
 * The actual waiting is performed by @ref waitForSignal.
 *
 * This function be called at any time before @ref waitForSignal, for
 * example calling it at the very beginning of the program is fine. If
 * such a program receives a signal early (i.e. before the @ref
 * waitForSignal call), the program will continue execution until @ref
 * waitForSignal is called and immediately continue there, for example
 * by shutting down.
 *
 * @param signals The non-empty set of signals to wait for. Should be
 *                constructed by applying bitwise-or to elements of
 *                the @ref Signal enumeration.
 *
 * @throw std::runtime_error If initialization fails, for example due
 *                           to unavailable resources.
 * @throw std::program_error If @a signals is 0, corresponding to the
 *                           empty set of signals.
 *
 * @author jmoringe
 */
RSC_EXPORT void initSignalWaiter(int signals
                                 = INTERRUPT_REQUESTED | TERMINATE_REQUESTED);

/**
 * Block until one of the signals specified in @ref initSignalWaiter
 * arrives, then return the signal.
 *
 * @return The received signal as a member of the @ref Signal
 *         enumeration.
 *
 * @throw std::logic_error If @ref initSignalWaiter has not been
 *                         called.
 * @throw std::runtime_error If waiting for signals fails.
 *
 * @author jmoringe
 */
RSC_EXPORT Signal waitForSignal();

/**
 * Return @c true if one of the signals requested in @ref
 * initSignalWaiter has arrived.
 *
 * @throw std::logic_error If @ref initSignalWaiter has not been
 *                         called.
 *
 * @author jmoringe
 */
RSC_EXPORT bool hasSignalArrived();

/**
 * Return suggested exit code for exiting the program after receiving
 * signal @a signal.
 *
 * @param signal The received signal as a member of the @ref Signal
 *               enumeration.
 *
 * @return The suggested exit code.
 *
 * @author jmoringe
 */
RSC_EXPORT int suggestedExitCode(Signal signal);

}
}
