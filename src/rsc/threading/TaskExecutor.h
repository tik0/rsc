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

#include <map>
#include <boost/thread.hpp>

#include "Task.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace threading {

/**
 * Interface for different scheduling strategies for Task instances.
 *
 * Implementations must not schedule the task if it is already canceled when
 * being scheduled.
 *
 * Implementations must execute the task even if it is canceled before within
 * the time of the specified delay.
 *
 * @author swrede
 * @author jwienke
 */
class RSC_EXPORT TaskExecutor {
public:

    /**
     * Schedules the new task.
     *
     * @param t the new task to schedule
     * @throw std::invalid_argument task to schedule is already canceled
     */
    virtual void schedule(TaskPtr t) = 0;

    /**
     * Schedules a new task to be executed after the specified delay.
     *
     * @param t new task to schedule
     * @param delayMus the delay after which the task should start
     * @throw rsc::misc::UnsupportedOperationException
     *             implementations may throw this exception to indicate that a
     *             scheduling of tasks with a specified delay is not supported
     * @throw std::invalid_argument task to schedule is already canceled
     */
    virtual void schedule(TaskPtr t, const boost::uint64_t& delayMus) = 0;

};

typedef boost::shared_ptr<TaskExecutor> TaskExecutorPtr;

}
}

