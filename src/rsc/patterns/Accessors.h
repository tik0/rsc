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

#include <boost/shared_ptr.hpp>

namespace rsc {
namespace patterns {

struct pass_through {
};

/**
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename T>
struct dereferencer {
    typedef T& result_type;

    inline result_type operator()(T* t) const {
        return *t;
    }

    inline const result_type operator()(const T* t) const {
        return *t;
    }
};

/**
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename T>
struct shared_ptr_dereferencer {
    typedef T& result_type;

    inline result_type operator()(boost::shared_ptr<T> t) const {
        return *t;
    }

    inline const result_type operator()(boost::shared_ptr<const T> t) const {
        return *t;
    }
};

}
}
