/* ============================================================
 *
 * This file is a part of the RSC project
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

#include "rsc/rscexports.h"

namespace rsc {

class RSC_EXPORT RSCVersion {
public:

    static unsigned int major();
    static unsigned int minor();
    static unsigned int patch();

    static std::string string();

    static unsigned int abi();

    static std::string buildId();
    static std::string buildDate();

    static std::string buildString();

private:
    RSCVersion();

};

}
