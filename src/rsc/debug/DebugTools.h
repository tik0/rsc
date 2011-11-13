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
#include <sstream>

#include <boost/shared_ptr.hpp>

#include "../runtime/TypeStringTools.h"
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
     * @return backtrace string as vector of method calls
     */
    virtual std::vector<std::string> createBacktrace(
            const unsigned int& maxElements = 20) = 0;

    std::string formatBacktrace(const std::vector<std::string>& trace);

    /**
     * Generates a string giving verbose information about an exception in the
     * given context. Use this immediately after catching the exception.
     *
     * @param e the exception to analyze
     * @tparam ExceptionType type of the caught exception
     */
    template<class ExceptionType>
    std::string exceptionInfo(const ExceptionType& e) {

        std::stringstream s;
        s << "Exception of type: " << rsc::runtime::typeName(e) << std::endl;
        s << "Backtrace:" << std::endl;
        std::vector<std::string> trace = createBacktrace();
        s << formatBacktrace(trace);
        return s.str();

    }

protected:

    DebugTools();

};

}
}

