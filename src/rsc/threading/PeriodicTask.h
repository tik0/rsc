/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#pragma once

#include "Task.h"

#include <iostream>

namespace rsc {
namespace threading {

/**
 * A specialization of Task that executes a task in a periodic manner by
 * providing an special implementation of #continueExec. A fixed interval
 * is guaranteed.
 *
 * @author swrede
 * @author jwienke
 * @todo remove header implementations
 * @todo jwienke: normally I would prefer a decorator type pattern instead of
 *       inheritance to make something periodic, but this would make the loop
 *       already provided in Task completely useless. We could simply introduce
 *       the loop in this decorator and have the default use an empty sleep
 *       time.
 */
class PeriodicTask: public Task {
public:

    PeriodicTask(const unsigned int &ms) :
        cycleTime(ms), logger(rsc::logging::Logger::getLogger(
                "rsc.threading.PeriodicTask")) {
    }

    virtual ~PeriodicTask() {
        RSCTRACE(logger, "~PeriodicTask() entered");
    }

    virtual bool continueExec() {
        RSCTRACE(logger, "~PeriodicTask()::continueExec() entered");

        if (isCancelRequested()) {
            return false;
        }

        // wait, give others a chance
        if (cycleTime != 0) {
            // TODO this does not really guarantee a fixed scheduling interval
            // we need to store the last time as a class member and constantly
            // increase it
            boost::xtime time;
            xtime_get(&time, boost::TIME_UTC);
            time.nsec += cycleTime * 1000000;
            // TODO provide option to interrupt in cancel using boost::this_thread
            try {
                RSCTRACE(logger, "PeriodicTask()::continueExec() before thread sleep, sleeping " << cycleTime << " ms");
                boost::this_thread::sleep(time);
            } catch (const boost::thread_interrupted &e) {
                // TODO handle interruption somewhere directly in task. This is
                // something all tasks should benefit of, see above TODO.
                RSCWARN(logger, "PeriodicTask()::continueExec() caught boost::thread_interrupted exception");
                return false;
            }
            RSCTRACE(logger, "PeriodicTask()::continueExec() thread woke up");
        }
        RSCTRACE(logger, "PeriodicTask()::continueExec() before lock");

        return true;

    }

private:

    unsigned int cycleTime;
    rsc::logging::LoggerPtr logger;

};

}
}

