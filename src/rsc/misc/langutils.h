/* ============================================================
 *
 * This file is a part of RSC project
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

#pragma once

#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "rsc/rscexports.h"

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
class RSC_EXPORT NullDeleter {
public:

    /**
     * Does nothing.
     *
     * @param ignored ignored
     */
    void operator()(void* ignored) const;

};

/**
 * A deleter for boost::shared_ptr which enables to use a pointer in a
 * shared_ptr, which is not a shared_ptr itself and owned by another object. The
 * only requirement is that this object in turn is maintained by a shared_ptr.
 * This deleter effectively keeps up the reference count of the parent object
 * by storing an instance of a shared_ptr to the parent until the child ptr is
 * deleted.
 *
 * @author jwienke
 * @tparam ParentType type of the parent which is owned by a shared_ptr
 */
template<class ParentType>
class ParentSharedPtrDeleter {
public:
    ParentSharedPtrDeleter(boost::shared_ptr<ParentType> parent) :
            parent(parent) {
    }

    void operator()(void const *) {
        parent.reset();
    }

private:
    boost::shared_ptr<ParentType> parent;
};

/**
 * Returns the current system time as milliseconds.
 *
 * @return current system time in milliseconds
 */
RSC_EXPORT boost::uint64_t currentTimeMillis();

/**
 * Returns the current system time as microseconds.
 *
 * @return current system time in microseconds
 */
RSC_EXPORT boost::uint64_t currentTimeMicros();

/**
 * Generates a random alpha-numeric character.
 *
 * @return alpha-numeric character
 */
RSC_EXPORT char randAlnumChar();

/**
 * Generates a random alpha-numeric string with fixed length.
 *
 * @param length desired length of the string
 * @return random string of specified length
 */
RSC_EXPORT std::string randAlnumStr(const std::string::size_type& length);

// This macro allows you to mark a function as being deprecated. Please note
// that if your return type contains a comma (template) you need to typedef it.
// Otherwise the commas are interpreted as arguments to this macro.
// Usage:
// DEPRECATED(void deprecatedFunction(int foo));
// void deprecatedFunction(int foo) {
//     int bla = foo + 1;
//     bla = bla + 1;
// }
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

}
}

