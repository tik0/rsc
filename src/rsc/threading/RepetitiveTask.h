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

#include <iostream>
#include <set>

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/timer.hpp>

#include "Task.h"

namespace rsc {
namespace threading {

/**
 * A task that already provides a base frame for common logic of interruptible
 * tasks. This is a loop that executes at least once. More times are possible
 * if #continueExec returns true after a loop. This is a common pattern for
 * interruptible tasks.
 *
 * In each iteration for a task, first a hook is called before the real work
 * (#pre), then the real work in that iteration is executed with #exec, and
 * afterwards a post hook (#post) is called. If another iteration shall be
 * performed, #continueExec has to return @c true (defaults to @c false).
 *
 * A default flag mechanism to cancel a task is provided and incorporated into
 * #continueExec.
 *
 * @author swrede
 * @author jwienke
 */
class RSC_EXPORT RepetitiveTask: public Task {
public:

    RepetitiveTask();

    virtual ~RepetitiveTask();

    /**
     * A method called before each iteration of the task. The default
     * implementation starts a timer for performance measurements. Override this
     * for comparable tasks.
     */
    virtual void pre();

    /**
     * This method is invoked to perform the real work of the task.
     */
    virtual void execute() = 0;

    /**
     * A method called after each iteration of the task. The default methods
     * logs timing information. Use it for comparable tasks only.
     */
    virtual void post();

    /**
     * Interrupts the task. The default implementation sets a boolean flag that
     * can be checked with #isCancelRequested.
     */
    virtual void cancel();

    /**
     * Tells whether the task was interrupted by a call to #cancel. The default
     * implementation returns the value of a boolean flag. Override this for
     * more complex logic but don't forget to also override #cancel then.
     *
     * @return @c true if #cancel was called, else @c false
     */
    virtual bool isCancelRequested();

    /**
     * Tells whether the #exec method shall be called another time. Do not
     * forget to incorporate the cancel mechanism while overriding this method
     * if the #exec method does not do this itself.
     *
     * @return @c true of iterations of #exec calls shall continue, else
     *         @c false, default implementation returns @c true unless the task
     *         is canceled.
     */
    virtual bool continueExec();

    /**
     * Performs the real task work orchestrating the iterative loop with the
     * different hook methods and so on.
     */
    void run();

    virtual void waitDone();
    virtual bool isDone();

    friend std::ostream& operator<<(std::ostream& out, const RepetitiveTask& t);

protected:

    mutable boost::recursive_mutex doneMutex;
    boost::condition doneCondition;

private:

    void markDone();

    void timerBeforeCycle();

    void timerAfterCycle();

    volatile bool cancelRequest;
    volatile bool cancelled;
    volatile bool done;

    rsc::logging::LoggerPtr logger;
    // TODO boost::timer measures cpu time, not really used time
    boost::timer timer;

};

std::ostream& operator<<(std::ostream& out, const RepetitiveTask& t);

}
}

