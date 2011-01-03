/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <boost/shared_ptr.hpp>

namespace rsc {
namespace threading {

class Task;
typedef boost::shared_ptr<Task> TaskPtr;

/**
 * Observer interface for the different states of a Task.
 *
 * @note Do not store boost::share_ptr instances to the observed task in an
 *       observer. Otherwise a dependency circle is created and the memory
 *       management does not work. If this is really needed (the task is always
 *       passed in in each method), use boost::weak_ptr.
 * @note processing in observers must be really fast as it is executed in the
 *       thread of the task and can otherwise block the task.
 * @author jwienke
 */
class TaskStateObserver {
public:

    /**
     * Called when a task starts processing.
     *
     * @param task the observed task
     */
    virtual void started(TaskPtr task) = 0;

    /**
     * Called when a new iteration of a task starts
     *
     * @param task the observed task
     */
    virtual void iterationStart(TaskPtr task) = 0;

    /**
     * Called when one iteration of a task has finished.
     *
     * @param task the observed task
     */
    virtual void iterationEnd(TaskPtr task) = 0;

    /**
     * Called when a task has finished processing, either through being
     * canceled or by a normal finish.
     *
     * @param task the observed task
     */
    virtual void done(TaskPtr task) = 0;

};

typedef boost::shared_ptr<TaskStateObserver> TaskStateObserverPtr;

}
}
