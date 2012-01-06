/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
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

#include "ContainerIO.h"

namespace std {
namespace detail {

void pair_style_delete(ios_base::event event_, ios_base& stream, int /*index*/) {
    if (event_ == ios_base::erase_event && stream.pword(
            pair_style::stream_storage)) {
        delete reinterpret_cast<pair_style*> (stream.pword(
                pair_style::stream_storage));
    }
}

pair_style::pair_style(const string& open_brace, const string& separator,
        const string& close_brace) :
    open_brace(open_brace), separator(separator), close_brace(close_brace) {
}

const int pair_style::stream_storage = ios_base::xalloc();

}

const detail::set_pair_style<detail::pair_style> pair_default = {
        detail::pair_style() };
const detail::set_pair_style<detail::pair_style> pair_whitespace = {
        detail::pair_style("", " ", "") };

}
