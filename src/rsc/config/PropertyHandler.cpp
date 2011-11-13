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

#include "PropertyHandler.h"

#include "TypedValue.h"

using namespace std;

using namespace rsc::runtime;

namespace rsc {
namespace config {

const Properties& PropertyHandler::getProperties() const {
    return this->result;
}

void PropertyHandler::handleOption(const vector<string>& key,
                                   const string& value) {
    this->result[key[0]] = parseTypedValue(value);
}

}
}
