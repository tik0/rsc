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

#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <string>
#include <boost/shared_ptr.hpp>

namespace rsc {
namespace misc {

/**
 * A simple timer to measure the execution time of algorithms.
 *
 * @author swrede
 */
class Timer {
public:

    Timer();
    Timer(const std::string &id);
    virtual ~Timer();
    void start();
    void stop();
    void reset();
    // return elapsed time in milliseconds
    double getElapsed();
    std::string getName();

private:
    std::string name;
    bool running;
    struct timeval tv_start, tv_end;
    struct timezone tz;

};

typedef boost::shared_ptr<Timer> TimerPtr;

}
}

#endif /* TIMER_H_ */
