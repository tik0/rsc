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

namespace rsc {
namespace debug {

/**
 * Linux GCC based implementation of DebugTools.
 *
 * @author jwienke
 */
class RSC_EXPORT LinuxDebugTools: public DebugTools {
public:
    LinuxDebugTools();
    virtual ~LinuxDebugTools();

    std::vector<std::string> createBacktrace(const unsigned int &maxElements);

};

}
}

