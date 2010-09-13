/* ============================================================
 *
 * This file is a part of RSC project
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

#ifndef RSCLANGUTILS_H_
#define RSCLANGUTILS_H_

#include <string>

#include <boost/cstdint.hpp>

namespace rsc {
namespace misc {

/**
 * A deleter object that can be used with boost::shared_ptr that doesn't release
 * any memory. This may be useful if you need to transfer a variable that is
 * allocated on the stack into a boost::shared_ptr to fulfill the interface
 *  requirements.
 *
 * @author jwienke
 */
class NullDeleter {
public:

    /**
     * Does nothing.
     *
     * @param ignored ignored
     */
    void operator()(void *ignored) const;

};

/**
 * Returns the current system time as milliseconds.
 *
 * @return current system time in milliseconds
 */
boost::uint64_t currentTimeMillis();

/**
 * Generates a random alpha-numeric character.
 *
 * @return alpha-numeric character
 */
char randAlnumChar();

/**
 * Generates a random alpha-numeric string with fixed length.
 *
 * @param length desired length of the string
 * @return random string of specified length
 */
std::string randAlnumStr(const std::string::size_type &length);

}
}

#endif /* RSCLANGUTILS_H_ */
