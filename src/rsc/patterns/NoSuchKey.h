/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
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

#include <boost/any.hpp>

#include "../runtime/NoSuchObject.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace patterns {

/**
 * This exception is thrown if a specified key does not exist in an associative
 * container.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
class RSC_EXPORT NoSuchKey: public runtime::NoSuchObject {
public:

    /**
     * Constructs a new no_such_key exception which indicates the that an
     * invalid key was used to query an associative container.
     *
     * @param message A string describing the invalid access.
     */
    NoSuchKey(const std::string& message);

};

}
}
