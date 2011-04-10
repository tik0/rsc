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

#include "Subprocess.h"

#include <boost/static_assert.hpp>

#if defined(SUBPROCESS_UNIX)
#include "UnixSubprocess.h"
#elif defined(SUBPROCESS_WINDOWS)
#include "WindowsSubprocess.h"
#endif

using namespace std;

namespace rsc {
namespace subprocess {

Subprocess::Subprocess() {
}

Subprocess::~Subprocess() {
}

SubprocessPtr Subprocess::newInstance(const string &command,
            const vector<string> &args) {
#if defined(SUBPROCESS_UNIX)
    return SubprocessPtr(new UnixSubprocess(command, args));
#elif defined(SUBPROCESS_WINDOWS)
    return SubprocessPtr(new WindowsSubprocess(command, args));
#else
    // No subprocess module available for this architecture
    BOOST_STATIC_ASSERT(false);
#endif
}

}
}
