/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
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

UUID::UUID(bool random) :
    id(random ? randomGen() : nilGen()) {
}

UUID::UUID(const string &uuid) {
    if (uuid != "") {
        boost::uuids::string_generator gen;
        id = gen(uuid);
    }
}

UUID::UUID(const char *uuid) {
    if (uuid != string("")) {
        boost::uuids::string_generator gen;
        id = gen(uuid);
    }
}

UUID::UUID(boost::uint8_t *data) {
    memcpy(id.data, data, 16);
}

UUID::UUID(const UUID &ns, const string &name) :
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

bool UUID::operator==(const UUID &other) const {
    return !(*this < other) && !(other < *this);
}

bool UUID::operator!=(const UUID &other) const {
    return !operator==(other);
}

bool UUID::operator<(const UUID &other) const {
    return id < other.id;
}

ostream &operator<<(ostream &stream, const UUID &id) {
    return stream << "UUID[" << id.id << "]";
}

}
}
