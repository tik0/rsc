/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of the
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

#include "UUID.h"

#include <sstream>

#include <boost/uuid/uuid_io.hpp>

using namespace std;

namespace rsc {
namespace misc {

boost::uuids::nil_generator UUID::nilGen =
    boost::uuids::nil_generator();

boost::uuids::basic_random_generator<boost::mt19937> UUID::randomGen =
        boost::uuids::basic_random_generator<boost::mt19937>();

UUID::UUID(const bool& random) :
    id(nilGen()) {
    if (random) {
        id = randomGen();
    }
}

UUID::UUID(const string& uuid) {
    if (uuid != "") {
        boost::uuids::string_generator gen;
        id = gen(uuid);
    }
}

UUID::UUID(const char* uuid) {
    if (uuid != string("")) {
        boost::uuids::string_generator gen;
        id = gen(uuid);
    }
}

UUID::UUID(boost::uint8_t* data) {
    memcpy(id.data, data, 16);
}

UUID::UUID(const UUID& ns, const string& name) :
    id(boost::uuids::name_generator(ns.getId())(name)) {
}

UUID::~UUID() {
}

boost::uuids::uuid UUID::getId() const {
    return id;
}

string UUID::getIdAsString() const {
    std::stringstream ss;
    ss << id;
    return ss.str();
}

bool UUID::operator==(const UUID& other) const {
    return !(*this < other) && !(other < *this);
}

bool UUID::operator!=(const UUID& other) const {
    return !operator==(other);
}

bool UUID::operator<(const UUID& other) const {
    return id < other.id;
}

ostream& operator<<(ostream& stream, const UUID& id) {
    return stream << "UUID[" << id.id << "]";
}

}
}
