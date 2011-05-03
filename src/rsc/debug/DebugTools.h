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

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "rsc/rscexports.h"

namespace rsc {
namespace debug {

class DebugTools;
typedef boost::shared_ptr<DebugTools> DebugToolsPtr;

/**
 * A class providing platform-specific runtime debugging tools, e.g. to generate
 * a backtrace.
 */
class RSC_EXPORT DebugTools {
public:

    virtual ~DebugTools();

    static DebugToolsPtr newInstance();

    /**
     * Generates a string representation of the current thread's backtrace.
     *
     * @return backtrace string as vector of method class
     */
    virtual std::vector<std::string> createBacktrace() = 0;

protected:

    DebugTools();

};

}
}

