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

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

#include "Task.h"

namespace rsc {
namespace threading {

/**
 * A Task subclass which maintains interruption through a volatile boolean flag
 * which should be processed by the user code.
 *
 * Implementations of #run must call #markDone whenever they stop working. This
 * is either because their job finished or they were canceled.
 *
 * @author jwienke
 */
class SimpleTask: public Task {
public:

    SimpleTask();
    virtual ~SimpleTask();

    virtual void cancel();

    virtual bool isCancelRequested();

    virtual void waitDone();

    virtual bool isDone();

protected:
    void markDone();

private:

    mutable boost::recursive_mutex mutex;
    boost::condition condition;

    volatile bool canceled;
    volatile bool done;

};

}
}
