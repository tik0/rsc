/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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

#include "OptionHandler.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/** Implementations of this interface obtain configuration information
 * somewhere and pass individual configuration options to an
 * @ref OptionHandler instance.
 *
 * @author jmoringe
 */
class RSC_EXPORT ConfigSource {
 public:
    /** Implementations should pass all configuration options to
     * @a handler.
     *
     * \param handler The handler to which all option should be passed.
     */
    virtual void emit(OptionHandler& handler) = 0;
};

}
}
