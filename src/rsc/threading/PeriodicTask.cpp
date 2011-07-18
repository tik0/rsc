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

#include "PeriodicTask.h"

using namespace std;

namespace rsc {
namespace threading {

PeriodicTask::PeriodicTask(const unsigned int &ms, bool accountProcTime) :
            cycleTime(ms), logger(rsc::logging::Logger::getLogger(
                            "rsc.threading.PeriodicTask")),
            fixedScheduling(accountProcTime), processingStart(),
            processingDuration(0) {
}

PeriodicTask::~PeriodicTask() {
    RSCTRACE(logger, "~PeriodicTask() entered");
}

bool PeriodicTask::continueExec() {
    RSCTRACE(logger, "~PeriodicTask()::continueExec() entered");

    if (isCancelRequested()) {
        return false;
    }

    // wait, give others a chance
    if (cycleTime != 0) {
        boost::xtime time;
        xtime_get(&time, boost::TIME_UTC);

        if (this->fixedScheduling) {
            this->processingDuration = rsc::misc::currentTimeMicros()
                    - this->processingStart;
        }

        time.nsec += cycleTime * 1000000 - this->processingDuration * 1000;
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

void PeriodicTask::pre() {
    if (this->fixedScheduling) {
        this->processingStart = rsc::misc::currentTimeMicros();
    }
    RepetitiveTask::pre();
}

}
}
