/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
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

#include <iostream>

#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/threading/Task.h"
#include "rsc/threading/PeriodicTask.h"
#include "rsc/threading/ThreadedTaskExecutor.h"
#include "rsc/logging/LoggerFactory.h"

using namespace std;
using namespace rsc::threading;
using namespace testing;

// TODO this is a really bad test case as it does not test the units on its own
//      provide more fine-grained tests first

class PeriodicTaskTest: public PeriodicTask {
public:
    PeriodicTaskTest() :
        PeriodicTask(50), i(0) {
    }

    boost::mutex m;
    boost::condition cond;

    void execute() {

        cout << "BLAAAA" << endl;

        boost::mutex::scoped_lock lock(m);

        i++;
        // cout << i << " " << flush;
        if (i == 10) {
            //cout << endl;
            // Notify main thread
            cond.notify_all();
            // TODO discuss the following observation:
            // Wait for approval by main thread, without this extra wait,
            // member i sometimes has already been incremented by one when the main thread
            // checks for i==10. Why? And why didn't it happen before switch to gtest?
            // Hypothesis: This thread is fast and the second thread is now slower. IF
            //             execute is called before the next instruction in the main
            //             it can re-acquire the lock (see above) and increment i before
            //             the comparison is done in the main thread.
            // However: Shouldn't wait re-acquire the lock atomically such that this
            //          this thread shall block in line 41?
            cond.wait(lock);
        }
        if (i > 20) {
            FAIL() << "Race Condition Detected!" ;
        }

    }

    volatile int i;

};

TEST(TaskTest, testExecution)
{

    rsc::logging::LoggerFactory::getInstance().reconfigure(
            rsc::logging::Logger::LEVEL_ALL);

    boost::shared_ptr<PeriodicTaskTest> p =
            boost::shared_ptr<PeriodicTaskTest>(new PeriodicTaskTest());

    ThreadedTaskExecutor executor;
    executor.schedule(p);

    boost::mutex::scoped_lock lock(p->m);
    bool shutdown = false;
    while (!shutdown) {
        //cout << "Step " << p->i << endl << flush;
        while (p->i != 10) {
            p->cond.wait(lock);
        }
        //cout << "Condition satisfied in Step " << p->i << endl << flush;
        if (p->i == 10) {
            //cout << "Stopping Task!" << endl;
            p->cancel();
            lock.unlock();
            p->cond.notify_all();
            p->waitDone();
            shutdown = true;
        }
    }
    //cerr << "main finished" << endl;
}
