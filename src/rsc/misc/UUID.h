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

#ifndef UUID_H_
#define UUID_H_

#include <ostream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/shared_ptr.hpp>

namespace rsc {
namespace misc {

/**
 * Encapsulates the generation and handling of UUIDs.
 *
 * @author swrede
 */
class UUID {
public:

    /**
     * Creates a random UUID.
     */
	UUID();

	/**
	 * Parses a UUID from a string. Various default formats are accepted.
	 *
	 * @param uuid
	 * @throw std::runtime_error given string is not acceptable as a UUID
	 */
	UUID(std::string uuid);

	virtual ~UUID();

	/**
	 * Returns the contained UUID on boost format.
	 *
	 * @return uuid in boost format.
	 */
	boost::uuids::uuid getId();

	/**
	 * Returns a string representing the UUID.
	 *
	 * @return string representation of the UUID
	 */
	std::string getIdAsString() const;

	bool operator==(const UUID &other) const;
	bool operator!=(const UUID &other) const;
	bool operator<(const UUID &other) const;

	friend std::ostream &operator<<(std::ostream &stream, const UUID &id);

private:

	boost::uuids::uuid id;
	 // TODO refactor to singleton
	static boost::uuids::basic_random_generator<boost::mt19937> gen;

};

typedef boost::shared_ptr<UUID> UUIDPtr;

std::ostream &operator<<(std::ostream &stream, const UUID &id);

}
}

#endif /* UUID_H_ */
