/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
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

RSC_EXPORT void pair_style_delete(ios_base::event event_, ios_base& stream,
        int index);

struct RSC_EXPORT pair_style {
    string open_brace;
    string separator;
    string close_brace;

    static const int stream_storage;

    pair_style(const string& open_brace = "(", const string& separator = ", ",
            const string& close_brace = ")");
};

template<typename T>
struct set_pair_style {
    T value;
};

}

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

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const vector<T>& container) {
    typedef vector<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "#(";
    if (container.size() >= 1) {
        copy(container.begin(), container.end() - 1,
                ostream_iterator<value_type> (stream, ", "));
        stream << container.back();
    }
    stream << ")";
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const deque<T>& container) {
    typedef vector<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "d(";
    if (container.size() >= 1) {
        copy(container.begin(), container.end() - 1,
                ostream_iterator<value_type> (stream, ", "));
        stream << container.back();
    }
    stream << ")";
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const list<T>& container) {
    typedef list<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "[";
    if (container.size() >= 1) {
        copy(container.begin(), --container.end(),
                ostream_iterator<value_type> (stream, ", "));
        stream << container.back();
    }
    stream << "]";
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const set<T>& container) {
    typedef set<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "{";
    if (container.size() >= 1) {
        copy(++container.begin(), container.end(),
                ostream_iterator<value_type> (stream, ", "));
        stream << *container.begin();
    }
    stream << "}";
    return stream;
}

template<typename Ch, typename Tr, typename R, typename S>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const map<R, S>& container) {
    typedef map<R, S> container_type;

    stream << "{";
    for (typename container_type::const_iterator it = container.begin(); it
            != container.end();) {
        stream << it->first << ": " << it->second;
        if (++it != container.end())
            stream << "\n";
    }
    stream << "}";
    return stream;
}

template<typename Ch, typename Tr, typename R, typename S>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const multimap<R, S>& container) {
    typedef multimap<R, S> container_type;

    stream << "{";
    for (typename container_type::const_iterator it = container.begin(); it
            != container.end();) {
        stream << it->first << ": " << it->second;
        if (++it != container.end())
            stream << "\n";
    }
    stream << "}";
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const valarray<T>& container) {
    typedef valarray<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "(";
    for (unsigned int i = 0; i != container.size(); ++i) {
        stream << container[i];
        if (i != container.size() - 1)
            stream << ", ";
    }
    stream << ")";
    return stream;
}

template<typename Ch, typename Tr, typename T>
basic_ostream<Ch, Tr>&
operator<<(basic_ostream<Ch, Tr>& stream, const slice_array<T>& container) {
    typedef slice_array<T> container_type;
    typedef typename container_type::value_type value_type;

    stream << "s(";
    /*    for (unsigned int i = 0; i != container.size(); ++i) {
     stream << container[i];
     if (i != container.size() - 1)
     stream << ", ";
     }*/
    stream << ")";
    return stream;
}

}
