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

#include <boost/shared_ptr.hpp>

#include "../logging/Logger.h"
#include "../misc/langutils.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace threading {

/**
 * A task is a self.contained piece of work that can be executed by another
 * thread. Tasks can be canceled and interested clients can wait until a task
 * has finished its work.
 *
 * In this context a task is done whenever it's execution finished. This
 * includes cancellation.
 *
 * If your interruption logic does not requires asynchronous method invocations
 * and determining the done state is easy, you can use SimpleTask.
 *
 * @author swrede
 * @author jwienke
 */
class RSC_EXPORT Task {
public:

    virtual ~Task();

    /**
     * Interrupts the task as soon as possible. Will, at some time, wake up
     * waiting threads in #waitDone.
     */
    virtual void cancel() = 0;

    /**
     * Tells whether the task was interrupted by a call to #cancel.
     *
     * @return @c true if #cancel was called, else @c false
     */
    virtual bool isCancelRequested() = 0;

    /**
     * Performs the real task work. Should check for canceling as often as
     * possible. After completion or cancellation, #isDone must return @true and
     * waiting threads in #waitDone must be waked up.
     *
     * It is a good practice to check for cancellation as a first step in the
     * implementation of this method in order to exit fast if the task was
     * canceled before it actually started running.
     */
    virtual void run() = 0;

    /**
     * Indicates whether the task finished execution, either through being
     * canceled or through successfully ending. If @c true is returned, the
     * #run method of this task will not perform anything anymore.
     *
     * @return @c true if run will not perform anything anymore
     */
    virtual bool isDone() = 0;

    /**
     * Waits for the execution of this task to finish. This may either be caused
     * by a successful completion of the task or by being canceled. This method
     * must block until #run exits or it is sure that #run will not be called
     * anymore.
     *
     * Undefined behavior might occur if this method is called before the task
     * is scheduled. I.e. a calling thread might block infinitely.
     */
    virtual void waitDone() = 0;

};

typedef boost::shared_ptr<Task> TaskPtr;

}
}

