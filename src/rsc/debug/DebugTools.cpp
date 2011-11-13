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

#include "DebugTools.h"

#if defined(DEBUGTOOLS_LINUX)
#include "LinuxDebugTools.h"
#elif defined(SUBPROCESS_WINDOWS)
#include "WindowsDebugTools.h"
#endif

using namespace std;

namespace rsc {
namespace debug {

DebugTools::DebugTools() {
}

DebugTools::~DebugTools() {
}

DebugToolsPtr DebugTools::newInstance() {
#if defined(DEBUGTOOLS_LINUX)
    return DebugToolsPtr(new LinuxDebugTools);
#elif defined(SUBPROCESS_WINDOWS)
    return DebugToolsPtr(new WindowsDebugTools);
#endif
}

string DebugTools::formatBacktrace(const vector<string>& trace) {
    stringstream s;
    for (std::vector<std::string>::const_iterator traceIt = trace.begin(); traceIt
            != trace.end(); ++traceIt) {
        s << "\t" << *traceIt << std::endl;
    }
    return s.str();
}

}
}
