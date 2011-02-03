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

#include "ContainerIO.h"

namespace std {

namespace detail {

void pair_style_delete(ios_base::event event_, ios_base& stream, int index) {
    if (event_ == ios_base::erase_event && stream.pword(
            pair_style::stream_storage))
        delete reinterpret_cast<pair_style*> (stream.pword(
                pair_style::stream_storage));
}

pair_style::pair_style(const string& open_brace, const string& separator,
        const string& close_brace) :
    open_brace(open_brace), separator(separator), close_brace(close_brace) {
}

const int pair_style::stream_storage = ios_base::xalloc();

}

const detail::set_pair_style<detail::pair_style> pair_default;
const detail::set_pair_style<detail::pair_style> pair_whitespace = {
        detail::pair_style("", " ", "") };

}
