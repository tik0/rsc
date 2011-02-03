/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
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

#include <typeinfo>
#include <stdexcept>
#include <string>
#include <ostream>

#include <boost/type_traits.hpp>

#include <boost/format.hpp>

#include "rsc/runtime/Demangle.h"
#include "rsc/runtime/ContainerIO.h"

namespace boost {

template<typename T>
struct has_stream_output: public false_type {
};

template<>
struct has_stream_output<bool> : public true_type {
};

template<>
struct has_stream_output<char> : public true_type {
};

template<>
struct has_stream_output<unsigned char> : public true_type {
};

template<>
struct has_stream_output<short> : public true_type {
};

template<>
struct has_stream_output<unsigned short> : public true_type {
};

template<>
struct has_stream_output<int> : public true_type {
};

template<>
struct has_stream_output<unsigned int> : public true_type {
};

template<>
struct has_stream_output<long> : public true_type {
};

template<>
struct has_stream_output<unsigned long> : public true_type {
};

template<>
struct has_stream_output<float> : public true_type {
};

template<>
struct has_stream_output<double> : public true_type {
};

template<>
struct has_stream_output<char*> : public true_type {
};

template<>
struct has_stream_output<std::string> : public true_type {
};

template<typename T>
struct has_stream_output<std::vector<T> > : public has_stream_output<T>::type {
};

template<>
struct has_stream_output<std::type_info> : public true_type {
};

}

namespace rsc {
namespace runtime {

/** @brief Returns a (demangled) string representation of @a type.
 *
 * @param type The type that's name should be returned.
 *
 * @return Demangled type name of @a type.
 * @throw runtime_error If demangling the type's name fails.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
std::string
type_name(const std::type_info& type) throw (std::runtime_error);

/** @brief Returns a (demangled) string representation of the type
 *  of the template parameter.
 *
 * @return Demangled type name of the type of the template
 * parameter.
 * @throw runtime_error If demangling the type's name fails.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename T>
std::string
type_name() throw (std::runtime_error);

/** @brief Returns a (demangled) string representation of the type
 *  of @a object.
 *
 * @param object The object, the stringified type of which should be
 * returned.
 * @return Demangled type name of the type of @a object.
 * @throw runtime_error If demangling the type's name fails.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename T>
std::string
type_name(const T& object) throw (std::runtime_error);

/** @brief Returns one of two to strings depending on whether type
 * @a T is known to be able to support stream output (using
 * operator<<).
 *
 * @param known_type_string The string to be used if type @a T
 * supports stream output. This string may contain @c %1%
 * substrings that will be replaced by the result of writing @a
 * value to a stream.
 * @param unknown_type_string The string to be used if type @a T
 * does not support stream output. This string will be returned
 * unmodified.
 * @param value The value that is to be embedded in @a
 * known_type_string if that is possible.
 * @return
 * - @a known_type_string   - If type @a T supports stream output.
 * - @a unknown_type_string - otherwise.
 * @throw format_error If the format specified in @a
 * known_type_string is invalid.
 *
 * @note As it cannot be deduced automatically whether a type T has
 * @c operator<<(ostream,T) defined or not, work has to be done to
 * support user-defined types here.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename T>
std::string
type_string(const std::string& known_type_string,
        const std::string& unknown_type_string, const T& value)
        throw (boost::io::format_error);

}
}

namespace std {

template<typename Ch, typename Tr>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const type_info& type_info_);

}

// free function implementations

namespace rsc {
namespace runtime {

template<typename T>
std::string type_name() throw (std::runtime_error) {
    return demangle(typeid(T).name());
}

template<typename T>
std::string type_name(const T& object) throw (std::runtime_error) {
    return demangle(typeid(object).name());
}

template<typename T>
std::string do_type_string(const std::string& known_type_string,
        const std::string&, const T& value, boost::true_type)
        throw (boost::io::format_error) {
    return (boost::format(known_type_string) % value).str();
}

template<typename T>
std::string do_type_string(const std::string&,
        const std::string& unknown_type_string, const T&, boost::false_type)
        throw (boost::io::format_error) {
    return unknown_type_string;
}

template<typename T>
std::string type_string(const std::string& known_type_string,
        const std::string& unknown_type_string, const T& value)
        throw (boost::io::format_error) {
    return do_type_string(known_type_string, unknown_type_string, value,
            boost::has_stream_output<T>());
}

}
}

namespace std {

template<typename Ch, typename Tr>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const type_info& type_info_) {
    stream << rsc::runtime::type_name(type_info_);

    return stream;
}

}
