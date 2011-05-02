/* ============================================================
 *
 * This file is a part of the m3s project.
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "Printable.h"

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

namespace rsc {
namespace runtime {

void Printable::printContents(ostream &/*stream*/) const {
    // nothing to do here as a default
}

void Printable::print(ostream &stream) const {
    stream << getClassName() << "[";
    printContents(stream);
    stream << "]";
}

ostream &operator<<(ostream &stream, const Printable &record) {
    record.print(stream);
    return stream;
}

ostream &operator<<(ostream &stream, const Printable *record) {

    stream << "*";
    record->print(stream);
    stream << " at " << ((void*) record);

    return stream;

}
}

}
