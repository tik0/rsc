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

#include "ThreadedTaskExecutor.h"

#include <stdexcept>

using namespace std;

namespace rsc {
namespace threading {

ThreadedTaskExecutor::ThreadedTaskExecutor() {
}

ThreadedTaskExecutor::~ThreadedTaskExecutor() {
}

void ThreadedTaskExecutor::schedule(TaskPtr t) {
    this->schedule(t, 0);
}

void ThreadedTaskExecutor::schedule(TaskPtr t,
        const boost::uint64_t& delayMus) {
    if (t->isCancelRequested()) {
        throw std::invalid_argument("Task already canceled.");
    }
    boost::thread taskThread(
            boost::bind(ThreadedTaskExecutor::executeTask, t, delayMus));
    // detach the thread because all further operations can be done on the
    // task object and this executor does not have to care about the thread
    taskThread.detach();
}

void ThreadedTaskExecutor::executeTask(TaskPtr task,
        const boost::uint64_t& delayMus) {
    if (delayMus > 0) {
        boost::this_thread::sleep(boost::posix_time::microseconds(delayMus));
    }
    task->run();
}

}
}
