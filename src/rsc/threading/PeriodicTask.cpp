/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
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

#include "PeriodicTask.h"

using namespace std;

namespace rsc {
namespace threading {

PeriodicTask::PeriodicTask(const unsigned int& ms, bool accountProcTime) :
        cycleTime(ms), logger(
                rsc::logging::Logger::getLogger("rsc.threading.PeriodicTask")), fixedScheduling(
                accountProcTime), nextProcessingStart(0) {
}

PeriodicTask::~PeriodicTask() {
    RSCTRACE(logger, "~PeriodicTask() entered");
}

bool PeriodicTask::continueExec() {
    RSCTRACE(logger, "~PeriodicTask()::continueExec() entered");

    if (isCancelRequested()) {
        return false;
    }

    if (nextProcessingStart == 0) {
        nextProcessingStart = rsc::misc::currentTimeMicros() + cycleTime * 1000;
        return true;
    }

    // wait, give others a chance
    if (cycleTime != 0) {
        // TODO provide option to interrupt in cancel using boost::this_thread
        try {
            RSCTRACE(
                    logger,
                    "PeriodicTask()::continueExec() before thread sleep, sleeping " << cycleTime << " ms");
            if (fixedScheduling) {
                boost::this_thread::sleep(
                        boost::posix_time::microseconds(
                                nextProcessingStart
                                        - rsc::misc::currentTimeMicros()));
            } else {
                boost::this_thread::sleep(
                        boost::posix_time::microseconds(cycleTime * 1000));
            }
        } catch (const boost::thread_interrupted& e) {
            // TODO handle interruption somewhere directly in task. This is
            // something all tasks should benefit of, see above TODO.
            RSCWARN(
                    logger,
                    "PeriodicTask()::continueExec() caught boost::thread_interrupted exception");
            return false;
        }RSCTRACE(logger, "PeriodicTask()::continueExec() thread woke up");
    }RSCTRACE(logger, "PeriodicTask()::continueExec() before lock");

    nextProcessingStart += cycleTime * 1000;

    return true;
}

}
}
