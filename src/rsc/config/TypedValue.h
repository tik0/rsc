/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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

#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/** Parses @a value and returns a @ref boost::any object containing an
 * object of the type indicated in @a value.
 *
 * Supported types are bool, uint, int and double.
 *
 * @param value A string of the form <TYPE>VALUE.
 * @return A @ref boost::any object.
 * @throw invalid_argument If the type specification is malformed or
 * the specified type is unknown.
 *
 * @author jmoringe
 */
RSC_EXPORT boost::any parseTypedValue(const std::string &value);

}
}
