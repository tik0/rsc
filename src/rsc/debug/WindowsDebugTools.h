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

#include "DebugTools.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace debug {

/**
 * Windows implementation for debug tools.
 *
 * @author jwienke
 */
class RSC_EXPORT WindowsDebugTools: public DebugTools {
public:

    WindowsDebugTools();
    virtual ~WindowsDebugTools();

    std::vector<std::string> createBacktrace(const unsigned int &maxElements);

};

}
}

