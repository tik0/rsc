/* ============================================================
 *
 * This file is a part of RSC project
 *
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

#include "langutils.h"

#include <cstdlib>
#include <cctype>
#include <algorithm>

#ifndef WIN32
#include <sys/time.h>
#else
#include <windows.h>
#endif
#include <time.h>

namespace rsc {
namespace misc {

void NullDeleter::operator()(void */*ignored*/) const {
}

boost::uint64_t currentTimeMillis() {

#ifdef WIN32

    static const __int64 magic = 116444736000000000; // 1970/1/1
    SYSTEMTIME st;
    GetSystemTime(&st);
    FILETIME ft;
    SystemTimeToFileTime(&st,&ft); // in 100-nanosecs...
    __int64 t;
    memcpy(&t,&ft,sizeof t);
    return (t - magic)/10000; // scale to millis.

#else

    timeval tv;
    gettimeofday(&tv, NULL);

    // (second-value in milliseconds) + (microsecond value in milliseconds)
    return (((boost::uint64_t) tv.tv_sec) * 1000l) + (tv.tv_usec / 1000l);

#endif

}

char randAlnumChar() {
    char c;
    while (!std::isalnum(c = static_cast<char> (std::rand()))) {
    }
    return c;
}

std::string randAlnumStr(const std::string::size_type &length) {
    std::string s;
    s.reserve(length);
    generate_n(std::back_inserter(s), length, randAlnumChar);
    return s;
}

}
}
