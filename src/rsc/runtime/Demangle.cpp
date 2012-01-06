/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
 *
 * This file may be licensed under the terms of of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
 *
 * ============================================================ */

#include "Demangle.h"

#include <boost/format.hpp>

// GCC implementation
#if defined DEMANGLE_GCC
#include <cxxabi.h>

namespace rsc {
namespace runtime {

std::string demangle(const char* mangledSymbol) {

    // Try to demangle the symbol.
    int status;
    char* demangled_symbol_ =
            abi::__cxa_demangle(mangledSymbol, 0, 0, &status);

    // Check whether demangling worked.
    if (status == -1) {
        throw std::runtime_error(
                "out of memory when allocating buffer for demangling");
    }

    if (status == -2 || status == -3) {
        throw InvalidMangledName(boost::str(boost::format(
                "invalid mangled name: `%1%'") % mangledSymbol)); // TODO is buffer allocated or not?
    }

    // Convert result to string and free temporary buffer.
    std::string demangled_symbol(demangled_symbol_);
    free(demangled_symbol_);

    return demangled_symbol;

}

}
}

#elif defined DEMANGLE_MSVC
// MSVC implementation

namespace rsc {
namespace runtime {

std::string demangle(const char* mangled_symbol) {
    // MSVC does everything on its own with typeid.
    return mangled_symbol;
}

}
}

#else

namespace rsc {
namespace runtime {

std::string demangle(const char* mangled_symbol) {
    return std::string(boost::str(boost::format("<cannot demangle %1%>")
			% mangled_symbol));
}

}
}

#endif

namespace rsc {
namespace runtime {

std::string demangle(const std::string& mangled_symbol) {
    return demangle(mangled_symbol.c_str());
}

}
}
