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

#include <stdexcept>

namespace rsc {
namespace runtime {

/** @brief This exception indicates the special case of an invalid
 * argument where an invalid mangled name was passed to a function.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
class InvalidMangledName: public std::invalid_argument {
public:
    InvalidMangledName(const std::string& message);
};

}
}
