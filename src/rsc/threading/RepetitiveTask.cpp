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

#include "RepetitiveTask.h"

using namespace std;

namespace rsc {
namespace threading {

RepetitiveTask::RepetitiveTask() :
        cancelRequest(false), cancelled(false), done(false), logger(
                rsc::logging::Logger::getLogger("rsc.threading.RepetitiveTask")) {
}

RepetitiveTask::~RepetitiveTask() {
}

void RepetitiveTask::pre() {
    timerBeforeCycle();
}

void RepetitiveTask::post() {
    timerAfterCycle();
}

void RepetitiveTask::cancel() {
    cancelRequest = true;
}

bool RepetitiveTask::isCancelRequested() {
    return cancelRequest;
}

bool RepetitiveTask::continueExec() {
    return !isCancelRequested();
}

void RepetitiveTask::run() {

    // ensure that nothing happens in this task if we are canceled before
    // actually being called
    if (isCancelRequested()) {
        markDone();
        return;
    }

    RSCDEBUG(logger, "run() entered");
    do {

        // TODO add exception handling
        // call pre hook
        pre();
        // call template method
        execute();
        // call post hook
        post();

        RSCTRACE(logger, "run cycle done");

    } while (continueExec());

    markDone();

    RSCDEBUG(logger, "task finished");

}

void RepetitiveTask::markDone() {
    boost::recursive_mutex::scoped_lock lock(doneMutex);
    done = true;
    this->doneCondition.notify_all();
}

void RepetitiveTask::waitDone() {
    RSCDEBUG(logger, "waitDone() entered");
    boost::recursive_mutex::scoped_lock lock(doneMutex);
    RSCDEBUG(logger, "waitDone() after lock, before wait");
    while (!done) {
        this->doneCondition.wait(lock);
    }
    RSCDEBUG(logger, "waitDone() finished");
}

bool RepetitiveTask::isDone() {
    return done;
}

void RepetitiveTask::timerBeforeCycle() {
    timer.restart();
}

void RepetitiveTask::timerAfterCycle() {
    // calculate processing time for last cycle, last n cycle, variance...
    RSCTRACE(logger, "Times (last cycle = " << timer.elapsed() << "s)");
}

ostream& operator<<(ostream& out, const RepetitiveTask& t) {
    // TODO missing content to output
    return out << "RepetitiveTask[cancelRequest=" << t.cancelRequest << "]";
}

}
}
