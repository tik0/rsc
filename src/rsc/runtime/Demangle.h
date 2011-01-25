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

#include "rsc/runtime/InvalidMangledName.h"

namespace rsc { namespace runtime {

  /*! @brief This function takes the mangled name of a symbol and
   * returns the demangled name of the symbol.
   *
   * @param mangled_symbol The mangled name of the symbol.
   * @return The demangled name of the symbol.
   * @throw runtime_error
   * @throw InvalidMangledName
   */
  std::string
  demangle(const char* mangled_symbol) throw (std::runtime_error,
					      InvalidMangledName);

  /*! @brief This function takes the mangled name of a symbol and
   * returns the demangled name of the symbol.
   *
   * @param mangled_symbol The mangled name of the symbol.
   * @return The demangled name of the symbol.
   * @throw runtime_error
   * @throw InvalidMangledName
   */
  std::string
  demangle(const std::string& mangled_symbol) throw (std::runtime_error,
						     InvalidMangledName);

 } }
