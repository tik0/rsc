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

#include "RepetitiveTask.h"

using namespace std;

namespace rsc {
namespace threading {

RepetitiveTask::RepetitiveTask() :
    cancelRequest(false), cancelled(false), done(false), logger(
            rsc::logging::Logger::getLogger("rsc.threading.RepetitiveTask")) {
}

RepetitiveTask::~RepetitiveTask() {
}

void RepetitiveTask::pre() {
    timerBeforeCycle();
}

void RepetitiveTask::post() {
    timerAfterCycle();
}

void RepetitiveTask::cancel() {
    cancelRequest = true;
}

bool RepetitiveTask::isCancelRequested() {
    return cancelRequest;
}

bool RepetitiveTask::continueExec() {
    return !isCancelRequested();
}

void RepetitiveTask::run() {

    done = false;

    RSCTRACE(logger, "run() entered");
    do {

        // TODO add exception handling
        // call pre hook
        pre();
        // call template method
        execute();
        // call post hook
        post();

        RSCTRACE(logger, "run cycle done");

    } while (continueExec());

    boost::recursive_mutex::scoped_lock lock(doneMutex);
    RSCINFO(logger, "run() finished");
    done = true;
    this->doneCondition.notify_all();

}

void RepetitiveTask::waitDone() {
    RSCDEBUG(logger, "waitDone() entered");
    boost::recursive_mutex::scoped_lock lock(doneMutex);
    RSCDEBUG(logger, "waitDone() after lock, before wait");
    while (!done) {
        this->doneCondition.wait(lock);
    }
    RSCDEBUG(logger, "waitDone() finished");
}

void RepetitiveTask::timerBeforeCycle() {
    timer.restart();
}

void RepetitiveTask::timerAfterCycle() {
    // calculate processing time for last cycle, last n cycle, variance...
    RSCTRACE(logger, "Times (last cycle = " << timer.elapsed() << "s)");
}

ostream &operator<<(ostream &out, const RepetitiveTask &t) {
    // TODO missing content to output
    return out << "RepetitiveTask[cancelRequest=" << t.cancelRequest << "]";
}

}
}
