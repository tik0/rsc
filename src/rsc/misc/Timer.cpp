/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "rsc/misc/Timer.h"

#include <sys/time.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace rsc {
namespace misc {

Timer::Timer() :
    name(""), running(false) {
}

Timer::Timer(const string &id) :
    name(id), running(false) {
}

Timer::~Timer() {
}

void Timer::start() {
    gettimeofday(&tv_start, &tz);
    running = true;
}

void Timer::stop() {
    gettimeofday(&tv_end, &tz);
    running = false;
}

void Timer::reset() {
    running = false;
    tv_end = tv_start;
}

double Timer::getElapsed() {
    double ms = 0;
    // is reset, return 0
    if ((tv_end.tv_usec == tv_start.tv_usec) && (tv_end.tv_sec
            == tv_start.tv_sec))
        return ms;
    if (running) {
        struct timeval tv_interim;
        gettimeofday(&tv_interim, &tz);
        double t1, t2;
        t1 = (double) tv_start.tv_sec + (double) tv_start.tv_usec / (1000
                * 1000);
        t2 = (double) tv_interim.tv_sec + (double) tv_interim.tv_usec / (1000
                * 1000);

        ms = (t2 - t1) * 1000;
    } else {
        double t1, t2;
        t1 = (double) tv_start.tv_sec + (double) tv_start.tv_usec / (1000
                * 1000);
        t2 = (double) tv_end.tv_sec + (double) tv_end.tv_usec / (1000 * 1000);
        ms = (t2 - t1) * 1000;
    }
    return ms;
}

string Timer::getName() {
    return name;
}

}
}
