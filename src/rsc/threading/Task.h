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

#include <iostream>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/timer.hpp>

#include "../logging/Logger.h"
#include "TaskStateObserver.h"

namespace rsc {
namespace threading {

class Task;
typedef boost::shared_ptr<Task> TaskPtr;

// TODO what to do with this? Provide a task wrapper that maps to delegate
//      functions
//template < class R >
//typedef boost::shared_ptr<Task<R> > TaskPtr;
// TODO think about if its possible to pass a smart pointer to this object
//      to the callbacks
//typedef boost::function<void()> Callback;

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
 * A default flag mechanism to cancel a task is provided but not incorporated
 * into #continueExec, as there is only one iteration to perform as a default.
 * If #exec itself does not provide a cancel mechanism, incorporate the cancel
 * flag into #continueExec.
 *
 * @author swrede
 * @author jwienke
 * @todo remove header implementations
 */
class Task {
public:

    Task() :
        cancelRequest(false), cancelled(false), done(false), logger(
                rsc::logging::Logger::getLogger("rsc.threading.Task")) {
    }

    virtual ~Task() {
    }

    /**
     * A method called before each iteration of the task. The default
     * implementation starts a timer for performance measurements. Override this
     * for comparable tasks.
     */
    virtual void pre() {
        timerBeforeCycle();
    }

    /**
     * This method is invoked to perform the real work of the task.
     */
    virtual void execute() = 0;

    /**
     * A method called after each iteration of the task. The default methods
     * logs timing information. Use it for comparable tasks only.
     */
    virtual void post() {
        timerAfterCycle();
    }

    /**
     * Interrupts the task. The default implementation sets a boolean flag that
     * can be checked with #isCancelRequested.
     */
    virtual void cancel() {
        cancelRequest = true;
    }

    /**
     * Tells whether the task was interrupted by a call to #cancel. The default
     * implementation returns the value of a boolean flag. Override this for
     * more complex logic but don't forget to also override #cancel then.
     *
     * @return @c true if #cancel was called, else @c false
     */
    virtual bool isCancelRequested() {
        return cancelRequest;
    }

    /**
     * Tells whether the #exec method shall be called another time. Do not
     * forget to incorporate the cancel mechanism while overriding this method
     * if the #exec method does not do this itself.
     *
     * @return @c true of iterations of #exec calls shall continue, else
     *         @c false, default implementation returns @c false
     */
    virtual bool continueExec() {
        return false;
    }

    /**
     * Performs the real task work orchestrating the iterative loop with the
     * different hook methods and so on.
     *
     * @param thisTask this task
     */
    void run(boost::weak_ptr<Task> thisTask) {


        assert(TaskPtr(thisTask).get() == this);

        // notify observers
        for (std::set<TaskStateObserverPtr>::iterator it = observers.begin(); it
                != observers.end(); ++it) {
            (*it)->started(TaskPtr(thisTask));
        }

        done = false;

        RSCTRACE(logger, "run() entered"); // << *id);
        // TODO Think about returning an iterator to the results of execute here!
        do {

            // notify observers
            for (std::set<TaskStateObserverPtr>::iterator it =
                    observers.begin(); it != observers.end(); ++it) {
                (*it)->iterationStart(TaskPtr(thisTask));
            }

            // TODO add exception handling
            // call pre hook
            pre();
            // call template method
            execute();
            // call post hook
            post();

            // notify observers
            for (std::set<TaskStateObserverPtr>::iterator it =
                    observers.begin(); it != observers.end(); ++it) {
                (*it)->iterationEnd(TaskPtr(thisTask));
            }

            RSCTRACE(logger, "run cycle done");

        } while (continueExec());

        boost::recursive_mutex::scoped_lock lock(doneMutex);
        RSCINFO(logger, "run() finished");
        done = true;
        this->doneCondition.notify_all();

        // notify observers
        for (std::set<TaskStateObserverPtr>::iterator it = observers.begin(); it
                != observers.end(); ++it) {
            (*it)->done(TaskPtr(thisTask));
        }

    }

    /**
     * Waits for the execution of this task to finish.
     */
    virtual void waitDone() {
        RSCDEBUG(logger, "waitDone() entered");
        boost::recursive_mutex::scoped_lock lock(doneMutex);
        RSCDEBUG(logger, "waitDone() after lock, before wait");
        while (!done) {
            this->doneCondition.wait(lock);
        }
        RSCDEBUG(logger, "waitDone() finished");
    }

    friend std::ostream& operator<<(std::ostream& out, const Task &t) {
        return out << "Task[cancelRequest=" << t.cancelRequest << "]";
    }

protected:

    mutable boost::recursive_mutex doneMutex;
    boost::condition doneCondition;

private:

    void timerBeforeCycle() {
        timer.restart();
    }

    void timerAfterCycle() {
        // calculate processing time for last cycle, last n cycle, variance...
        RSCINFO(logger, "Times (last cycle = " << timer.elapsed() << "s)");
    }

    volatile bool cancelRequest;
    volatile bool cancelled;
    volatile bool done;

    rsc::logging::LoggerPtr logger;
    // TODO boost::timer measures cou time, not really used time
    boost::timer timer;

    std::set<TaskStateObserverPtr> observers;

};

}
}

