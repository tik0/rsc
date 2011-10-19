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

#include "SimpleTask.h"

namespace rsc {
namespace threading {

SimpleTask::SimpleTask() :
        canceled(false), done(false) {
}

SimpleTask::~SimpleTask() {
}

void SimpleTask::cancel() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    canceled = true;
}

bool SimpleTask::isCancelRequested() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return canceled;
}

void SimpleTask::waitDone() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    while (!done) {
        condition.wait(lock);
    }
}

void SimpleTask::markDone() {
    {
        boost::recursive_mutex::scoped_lock lock(mutex);
        done = true;
    }
    condition.notify_all();
}

bool SimpleTask::isDone() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return done;
}

}
}
