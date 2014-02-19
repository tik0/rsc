/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

/** @file */

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

/**
 * This macro allows you to mark a function as being deprecated including a
 * message explaining the deprecation.
 *
 * Usage:
 * @code
 * DEPRECATED_MSG(void deprecatedFunction(int foo), "use XYZ instead");
 * void deprecatedFunction(int foo) {
 *     int bla = foo + 1;
 *     bla = bla + 1;
 * }
 * @endcode
 *
 * @note If your return type contains a comma (template) you need to typedef it.
 * Otherwise the commas are interpreted as arguments to this macro.
 */
#ifdef __GNUC__
#define DEPRECATED_MSG(func, msg) func __attribute__ ((deprecated (#msg)))
#elif defined(_MSC_VER)
#define DEPRECATED_MSG(func, msg) __declspec(deprecated(#msg)) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED_MSG(func, msg) func
#endif

/**
 * This macro allows you to mark a function as being deprecated. A default
 * explanatory message will be used. Usually you should use #DEPRECATED_MSG
 * instead.
 *
 * Usage:
 * @code
 * DEPRECATED_MSG(void deprecatedFunction(int foo));
 * void deprecatedFunction(int foo) {
 *     int bla = foo + 1;
 *     bla = bla + 1;
 * }
 * @endcode
 *
 * @note If your return type contains a comma (template) you need to typedef it.
 * Otherwise the commas are interpreted as arguments to this macro.
 */
#define DEPRECATED(fun) DEPRECATED_MSG(fun, "Use of deprecated construct.")

/**
 * This macro allows you to mark a class as being deprecated.
 *
 * Usage:
 * @code
 * class DEPRECATED_CLASS("Your explanation") SimpleDeprecation;
 * class SimpleDeprecation {
 * };
 * @endcode
 *
 * @note On GCC, a bug might prevent deprecation warnings for template classes
 */
#ifdef __GNUC__
#define DEPRECATED_CLASS(msg) __attribute__ ((deprecated (#msg)))
#elif defined(_MSC_VER)
#define DEPRECATED_CLASS(msg) __declspec(deprecated(#msg))
#else
#pragma message("WARNING: You need to implement DEPRECATED_CLASS for this compiler")
#define DEPRECATED_CLASS(msg)
#endif

// provide static asserts that only issue a warning
// adapted from: http://stackoverflow.com/a/8990275/283649

#define PP_CAT(x,y) PP_CAT1(x,y)
#define PP_CAT1(x,y) x##y

namespace detail {

struct true_type {
};
struct false_type {
};
template<int test> struct converter: public true_type {
};
template<> struct converter<0> : public false_type {
};

}

/**
 * Generates a compiler warning in case a static condition is not held. Might
 * be used at namespace, structure, and function scope.
 *
 * Usage:
 * @code
 * STATIC_ASSERT_WARN(2==3, "Will always warn")
 * STATIC_ASSERT_WARN((!boost::is_same<T, Event>::value), "Illegal type used")
 * @endcode
 *
 * @note in case you are inside a templated class or function, you might need to
 *       use STATIC_ASSERT_WARN_TEMPLATE to really generate a warning.
 */
#define STATIC_ASSERT_WARN(cond, msg) \
struct PP_CAT(static_warning,__LINE__) { \
    DEPRECATED_MSG(void _(::rsc::misc::detail::false_type const& ),msg) {}; \
    void _(::rsc::misc::detail::true_type const& ) {}; \
    PP_CAT(static_warning,__LINE__)() {_(::rsc::misc::detail::converter<(cond)>());} \
}

/**
 * Generates a compiler warning in case a static condition is not held. Might
 * be used at namespace, structure, and function scope. The first argument must
 * be a completely unique token.
 *
 * Usage:
 * @code
 * STATIC_ASSERT_WARN_TEMPLATE(UNIQUE_TOKEN, 2==3, "Will always warn")
 * STATIC_ASSERT_WARN_TEMPLATE(UNIQUE_TOKEN, (!boost::is_same<T, Event>::value), "Illegal type used")
 * @endcode
 *
 * @note This macro adds a new member to your type. So try to use
 *       STATIC_ASSERT_WARN whenever possible
 */
#define STATIC_ASSERT_WARN_TEMPLATE(token, cond, msg) \
    STATIC_ASSERT_WARN(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)

}
}

