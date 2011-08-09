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

#pragma once

#include <ostream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/shared_ptr.hpp>

#include "rsc/rscexports.h"

namespace rsc {
namespace misc {

/**
 * Encapsulates the generation and handling of UUIDs.
 *
 * @author swrede
 */
class RSC_EXPORT UUID {
public:

    /**
     * Creates a new UUID object that is either random or the nil
     * UUID.
     *
     * @param random If @c true, a random UUID is created. Otherwise
     * the nil UUID is created.
     */
    explicit UUID(const bool &random = true);

    /**
     * Parses a UUID from a string. Various default formats are
     * accepted.
     *
     * @param uuid A string representation of the desired UUID.
     * @throw std::runtime_error given string is not acceptable as a
     * UUID
     */
    explicit UUID(const std::string &uuid);

    /**
     * Parses a UUID from a string. Various default formats are
     * accepted.
     *
     * @param uuid A string representation of the desired UUID.
     * @throw std::runtime_error given string is not acceptable as a
     * UUID
     */
    explicit UUID(const char *uuid);

    /**
     * Generates a uuid from the given 16 byte representation.
     *
     * @param data 16 byte representation of a uuid
     */
    explicit UUID(boost::uint8_t *data);

    /**
     * Generates a uuid for @a name in namespace @a ns.
     *
     * @param ns Namespace in which @a name should be placed.
     * @param name A unique name within namespace @a ns.
     */
    UUID(const UUID &ns, const std::string &name);

    virtual ~UUID();

    /**
     * Returns the contained UUID on boost format.
     *
     * @return uuid in boost format.
     */
    boost::uuids::uuid getId() const;

    /**
     * Returns a string representing the UUID.
     *
     * @return string representation of the UUID
     */
    std::string getIdAsString() const;

    bool operator==(const UUID &other) const;
    bool operator!=(const UUID &other) const;
    bool operator<(const UUID &other) const;

    friend RSC_EXPORT std::ostream &operator<<(std::ostream &stream, const UUID &id);

private:

    boost::uuids::uuid id;
    // TODO refactor to singleton
    static boost::uuids::nil_generator nilGen;
    static boost::uuids::basic_random_generator<boost::mt19937> randomGen;

};

typedef boost::shared_ptr<UUID> UUIDPtr;

RSC_EXPORT std::ostream &operator<<(std::ostream &stream, const UUID &id);

}
}
