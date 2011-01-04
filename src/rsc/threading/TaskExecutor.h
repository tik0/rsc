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
 * @author swrede
 * @author jwienke
 */
class RSC_EXPORT TaskExecutor {
public:

    /**
     * Schedules the new task.
     *
     * @param t the new task to schedule
     * @todo exception specification, which errors can occur?
     */
    virtual void schedule(TaskPtr t) = 0;

};

typedef boost::shared_ptr<TaskExecutor> TaskExecutorPtr;

}
}

