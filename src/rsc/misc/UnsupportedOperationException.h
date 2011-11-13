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

#include <stdexcept>

namespace rsc {
namespace misc {

/**
 * An exception which indicates that a certain operation is not supported by
 * the target.
 *
 * @author jwienke
 */
class UnsupportedOperationException: public std::runtime_error {
public:
    UnsupportedOperationException(const std::string& description);
    virtual ~UnsupportedOperationException() throw();
};

}
}
