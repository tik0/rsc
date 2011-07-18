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

#include "RepetitiveTask.h"

#include <rsc/misc/langutils.h>

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
 * @author anordman
 */
class RSC_EXPORT PeriodicTask: public RepetitiveTask {
public:

    /**
     * Constructs a new periodic task with a fixed wait time after each
     * iteration.
     *
     * @param ms time to wait between iterations. No overall-fixed interval
     *           is guaranteed by the implementation. Time is in milliseconds.
     * @param accountProcTime subtracts the processing time from sleep time in
     *           order to guarantee a fixed scheduling interval
     */
    PeriodicTask(const unsigned int &ms, bool accountProcTime = true);

    virtual ~PeriodicTask();

    /**
     * Implements a waiting logic for the continuation of the repetitive task.
     *
     * @return @ctrue until the task is canceled after having waited for the
     *         specified amount of time.
     */
    virtual bool continueExec();

private:

    /**
     * Logs time at processing start in order to calculate processing time. Also
     * calls pre method of super-class RepetitiveTask.
     */
    virtual void pre();

    unsigned int cycleTime;
    rsc::logging::LoggerPtr logger;
    bool fixedScheduling;
    boost::uint64_t processingStart;
    boost::uint64_t processingDuration;
};

}
}

