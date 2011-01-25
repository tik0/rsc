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

#include "Demangle.h"

#include <boost/format.hpp>

#include <cxxabi.h>

namespace rsc { namespace runtime {

  // free function implementations

  std::string
  demangle(const char* mangled_symbol) throw (std::runtime_error,
					      InvalidMangledName) {
#ifdef 1
    // Try to demangle the symbol.
    int status;
    char* demangled_symbol_ = abi::__cxa_demangle(mangled_symbol, 0, 0, &status);

    // Check whether demangling worked.
    if (status == -1)
      throw std::runtime_error("out of memory when allocating buffer for demangling");

    if (status == -2 || status == -3)
      throw InvalidMangledName(boost::str(boost::format("invalid mangled name: `%1%'")
					  % mangled_symbol)); // TODO is buffer allocated or not?

    // Convert result to string and free temporary buffer.
    std::string demangled_symbol(demangled_symbol_);
    free(demangled_symbol_);
#else
    std::string demangled_symbol(boost::str(boost::format("<cannot demangle %1%>")
					    % mangled_symbol));
#endif

    return demangled_symbol;
  }

  std::string
  demangle(const std::string& mangled_symbol) throw (std::runtime_error,
						     InvalidMangledName) {
    return demangle(mangled_symbol.c_str());
  }

} }
