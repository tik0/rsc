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

#include "LinuxDebugTools.h"

#include <execinfo.h>

using namespace std;

namespace rsc {
namespace debug {

LinuxDebugTools::LinuxDebugTools() {
}

LinuxDebugTools::~LinuxDebugTools() {
}

vector<string> LinuxDebugTools::createBacktrace(const unsigned int& maxElements) {

    void** arr = (void**) malloc(maxElements * sizeof(void*));
    int nSize = backtrace(arr, maxElements);
    char** sym = backtrace_symbols(arr, nSize);

    vector<string> result;
    for (int i = 0; i < nSize; i++) {
        result.push_back(sym[i]);
    }

    free(sym);
    free(arr);

    return result;

}

}
}
