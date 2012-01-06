/* ============================================================
 *
 * This file is a part of the m3s project.
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "Printable.h"

#include <iostream>
#include <string>

#include "TypeStringTools.h"

using namespace std;

namespace rsc {
namespace runtime {

Printable::~Printable() {
}

string Printable::getClassName() const {
    return typeName(*this);
}

void Printable::printContents(ostream& /*stream*/) const {
    // nothing to do here as a default
}

void Printable::print(ostream& stream) const {
    stream << getClassName() << "[";
    printContents(stream);
    stream << "]";
}

ostream& operator<<(ostream& stream, const Printable& record) {
    record.print(stream);
    return stream;
}

ostream& operator<<(ostream& stream, const Printable* record) {

    if (record) {

        stream << "*";
        record->print(stream);
        stream << " at " << ((void*) record);

    } else {
        stream << "0x0";
    }

    return stream;

}
}

}
