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

#include "TypedValue.h"

#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace std;

using namespace boost;

namespace rsc {
namespace config {

any parseTypedValue(const string& value) {
    if ((value[0] == '<') && (value.find('>') != string::npos)) {
        unsigned int index = value.find('>');
        string type = value.substr(1, index - 1);
        string rest = value.substr(index + 1);
        if (type == "bool") {
            return any(lexical_cast<bool>(rest));
        } else if (type == "uint") {
            return any(lexical_cast<unsigned int>(rest));
        } else if (type == "int") {
            return any(lexical_cast<int>(rest));
        } else if (type == "double") {
            return any(lexical_cast<double>(rest));
        } else {
            throw invalid_argument(str(format("Unknown option type `%1%'")
                                       % type));
        }
    } else {
        return any(value);
    }
}

}
}
