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

#include "Properties.h"

namespace rsc {
namespace runtime {

  // Properties implementation

  Properties&
  Properties::operator<<=(const Properties& other) {
    for (Properties::const_iterator it = other.begin();
         it != other.end(); ++it) {
      (*this)[it->first] = it->second;
    }

    return *this;
  }

  bool
  Properties::has(const std::string& name) const throw () {
    return find(name) != end();
  }

  // Free functions.

  Properties
  operator<<(const Properties& Properties1,
	     const Properties& Properties2) {
    Properties result;
    for (Properties::const_iterator it = Properties1.begin();
         it != Properties1.end(); ++it) {
      result[it->first] = it->second;
    }
    for (Properties::const_iterator it = Properties2.begin();
         it != Properties2.end(); ++it) {
      result[it->first] = it->second;
    }
    return result;
  }

} }
