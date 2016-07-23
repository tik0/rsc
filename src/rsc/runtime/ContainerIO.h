/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010-2016 Jan Moringen
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

#pragma once

#include <string>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <valarray>
#include <deque>

#include <algorithm>
#include <iterator>

#include <iostream>

#include "rsc/rscexports.h"

namespace std {
namespace detail {

// Style helpers for pairs

RSC_EXPORT void pair_style_delete(ios_base::event event_,
                                  ios_base&       stream,
                                  int             index);

struct RSC_EXPORT pair_style {
    string open_brace;
    string separator;
    string close_brace;

    static const int stream_storage;

    pair_style(const string& open_brace  = "(",
               const string& separator   = ", ",
               const string& close_brace = ")");
};

template<typename T>
struct set_pair_style {
    T value;
};

// Style helpers for element sequence types

RSC_EXPORT void element_sequence_style_delete(ios_base::event event_,
                                              ios_base&       stream,
                                              int             index);

struct RSC_EXPORT element_sequence_style {
    string separator;
    string first_separator;
    string last_separator;

    static const int stream_storage;

    element_sequence_style(const string& separator       = ", ",
                           const string& first_separator = "",
                           const string& last_separator  = "");
};

template<typename T>
struct set_element_sequence_style {
    set_element_sequence_style(const T& value) : value(value) {};
    T value;
};

// Style helpers for container types

RSC_EXPORT void container_style_delete(ios_base::event event_,
                                       ios_base&       stream,
                                       int             index);

struct RSC_EXPORT container_style {
    string open;
    string close;

    static const int stream_storage;

    container_style(const string& open,
                    const string& close);
};

template<typename T>
struct set_container_style {
    T value;
};

}

// Installing pair style

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream,
        const detail::set_pair_style<T>& style) {
    // Delete old style, if any, and install new style.
    if (stream.pword(detail::pair_style::stream_storage))
        delete reinterpret_cast<detail::pair_style*> (stream.pword(
                detail::pair_style::stream_storage));
    else
        stream.register_callback(&detail::pair_style_delete, 0);

    stream.pword(detail::pair_style::stream_storage) = new T(style.value);

    // Return the modified stream.
    return stream;
}

RSC_EXPORT extern const detail::set_pair_style<detail::pair_style> pair_default;
RSC_EXPORT extern const detail::set_pair_style<detail::pair_style>
        pair_whitespace;

// Installing element sequence style

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream,
        const detail::set_element_sequence_style<T>& style) {
    // Delete old style, if any, and install new style.
    if (stream.pword(detail::element_sequence_style::stream_storage))
        delete reinterpret_cast<detail::element_sequence_style*> (stream.pword(
                detail::element_sequence_style::stream_storage));
    else
        stream.register_callback(&detail::element_sequence_style_delete, 0);

    stream.pword(detail::element_sequence_style::stream_storage) = new T(style.value);

    // Return the modified stream.
    return stream;
}

RSC_EXPORT detail::set_element_sequence_style<detail::element_sequence_style>
element_sequence(const string& separator       = ", ",
                 const string& first_separator = "",
                 const string& last_separator  = "");

RSC_EXPORT extern const detail::set_element_sequence_style<detail::element_sequence_style> element_sequence_singleline;
RSC_EXPORT extern const detail::set_element_sequence_style<detail::element_sequence_style> element_sequence_multiline;

// Installing Container style

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream,
           const detail::set_container_style<T>& style) {
    // Delete old style, if any, and install new style.
    if (stream.pword(detail::container_style::stream_storage))
        delete reinterpret_cast<detail::container_style*> (stream.pword(
                                                               detail::container_style::stream_storage));
    else
        stream.register_callback(&detail::container_style_delete, 0);

    stream.pword(detail::container_style::stream_storage) = new T(style.value);

    // Return the modified stream.
    return stream;
}


RSC_EXPORT extern const detail::set_container_style<detail::container_style> container_none;
RSC_EXPORT extern const detail::set_container_style<detail::container_style> container_parentheses;
RSC_EXPORT extern const detail::set_container_style<detail::container_style> container_hash;
RSC_EXPORT extern const detail::set_container_style<detail::container_style> container_d;
RSC_EXPORT extern const detail::set_container_style<detail::container_style> container_brackets;
RSC_EXPORT extern const detail::set_container_style<detail::container_style> container_braces;

// Formatting pairs

template<typename Ch, typename Tr, typename R, typename S>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const pair<R, S>& pair) {
    // Try to retrieve the installed style implementation. Create one,
    // if none is installed.
    if (!stream.pword(detail::pair_style::stream_storage))
        stream << pair_default;

    detail::pair_style& style =
            *reinterpret_cast<detail::pair_style*> (stream.pword(
                    detail::pair_style::stream_storage));

    stream << style.open_brace << pair.first << style.separator << pair.second
            << style.close_brace;
    return stream;
}

// Formatting containers

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const vector<T>& container) {
    typedef vector<T> container_type;
    typedef typename container_type::value_type value_type;

    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>
              (stream.pword(detail::container_style::stream_storage))
           : container_hash.value);

    stream << container_style.open;
    if (container.size() >= 1) {
        stream << element_sequence_style.first_separator;
        copy(container.begin(), container.end() - 1,
             ostream_iterator<value_type>(stream,
                                          element_sequence_style.separator.c_str()));
        stream << container.back();
        stream << element_sequence_style.last_separator;
    }
    stream << container_style.close;
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const deque<T>& container) {
    typedef vector<T> container_type;
    typedef typename container_type::value_type value_type;

    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>
              (stream.pword(detail::container_style::stream_storage))
           : container_d.value);

    stream << container_style.open;
    if (container.size() >= 1) {
        stream << element_sequence_style.first_separator;
        copy(container.begin(), container.end() - 1,
             ostream_iterator<value_type>(stream,
                                          element_sequence_style.separator.c_str()));
        stream << container.back();
        stream << element_sequence_style.last_separator;
    }
    stream << container_style.close;
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const list<T>& container) {
    typedef list<T> container_type;
    typedef typename container_type::value_type value_type;

    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>
              (stream.pword(detail::container_style::stream_storage))
           : container_brackets.value);

    stream << container_style.open;
    if (!container.empty()) {
        stream << element_sequence_style.first_separator;
        copy(container.begin(), --container.end(),
             ostream_iterator<value_type>(stream,
                                          element_sequence_style.separator.c_str()));
        stream << container.back();
        stream << element_sequence_style.last_separator;
    }
    stream << container_style.close;
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const set<T>& container) {
    typedef set<T> container_type;
    typedef typename container_type::value_type value_type;

    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>(
              stream.pword(detail::container_style::stream_storage))
           : container_braces.value);

    stream << container_style.open;
    if (!container.empty()) {
        stream << element_sequence_style.first_separator;
        copy(++container.begin(), container.end(),
             ostream_iterator<value_type>(stream,
                                          element_sequence_style.separator.c_str()));
        stream << *container.begin();
        stream << element_sequence_style.last_separator;
    }
    stream << container_style.close;
    return stream;
}

template<typename Ch, typename Tr, typename R, typename S>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const map<R, S>& container) {
    typedef map<R, S> container_type;

    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>
              (stream.pword(detail::container_style::stream_storage))
           : container_braces.value);

    stream << container_style.open;
    stream << element_sequence_style.first_separator;
    for (typename container_type::const_iterator it = container.begin(); it
            != container.end();) {
        stream << *it;
        if (++it != container.end())
            stream << element_sequence_style.separator;
    }
    stream << element_sequence_style.last_separator;
    stream << container_style.close;
    return stream;
}

template<typename Ch, typename Tr, typename R, typename S>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const multimap<R, S>& container) {
    typedef multimap<R, S> container_type;

    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>
              (stream.pword(detail::container_style::stream_storage))
           : container_braces.value);

    stream << container_style.open;
    stream << element_sequence_style.first_separator;
    for (typename container_type::const_iterator it = container.begin(); it
            != container.end();) {
        stream << *it;
        if (++it != container.end())
            stream << element_sequence_style.separator;
    }
    stream << element_sequence_style.last_separator;
    stream << container_style.close;
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const valarray<T>& container) {
    if (!stream.pword(detail::element_sequence_style::stream_storage)) {
        stream << element_sequence_singleline;
    }

    detail::element_sequence_style& element_sequence_style =
            *reinterpret_cast<detail::element_sequence_style*>(stream.pword(
                    detail::element_sequence_style::stream_storage));
    detail::container_style container_style
        = (stream.pword(detail::container_style::stream_storage)
           ? *reinterpret_cast<detail::container_style*>
              (stream.pword(detail::container_style::stream_storage))
           : container_parentheses.value);

    stream << container_style.open;
    stream << element_sequence_style.first_separator;
    for (unsigned int i = 0; i != container.size(); ++i) {
        stream << container[i];
        if (i != container.size() - 1)
            stream << element_sequence_style.separator;
    }
    stream << element_sequence_style.last_separator;
    stream << container_style.close;
    return stream;
}

/* TODO(jmoringe, 2012-11-12): unfinished
template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const slice_array<T>& container) {
    typedef slice_array<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "s(";
    for (unsigned int i = 0; i != container.size(); ++i) {
        stream << container[i];
        if (i != container.size() - 1)
            stream << ", ";
    }
    stream << ")";
    return stream;
}*/

}
