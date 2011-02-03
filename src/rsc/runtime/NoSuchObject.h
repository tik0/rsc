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

#include <string>

namespace rsc { namespace runtime {

  /** @brief This exception is thrown if a specified object does not
   * exist.
   *
   * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
   */
  class NoSuchObject : public std::runtime_error { // TODO could be logic_error
  public:
    /** @brief Constructs a new @a NoSuchObject exception which
     * indicates the absence of the object specified by @a object.
     *
     * @param object The object which was specified but did not exist.
     */
    NoSuchObject(const std::string& object);
  };

} }
