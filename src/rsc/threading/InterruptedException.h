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

#pragma once

#include <stdexcept>

#include "rsc/rscexports.h"

namespace rsc {
namespace threading {

/**
 * An exception thrown if a blocking operation was interrupted.
 *
 * @author jwienke
 */
class RSC_EXPORT InterruptedException: public std::runtime_error {
public:
    InterruptedException();
    InterruptedException(const std::string& message);
    virtual ~InterruptedException() throw();
};

}
}

