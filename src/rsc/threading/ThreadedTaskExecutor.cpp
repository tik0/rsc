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

#include "ThreadedTaskExecutor.h"

namespace rsc {
namespace threading {

ThreadedTaskExecutor::ThreadedTaskExecutor() {
}

ThreadedTaskExecutor::~ThreadedTaskExecutor() {
}

void ThreadedTaskExecutor::schedule(TaskPtr t) {
    boost::thread taskThread(boost::bind(ThreadedTaskExecutor::executeTask, t));
    // detach the thread because all further operations can be done on the
    // task object and this executor does not have to care about the thread
    taskThread.detach();
}

void ThreadedTaskExecutor::executeTask(TaskPtr task) {
    task->run();
}

}
}
