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

#include <string>
#include <vector>

#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/** Implementations of this interface receive options from @ref
 * ConfigSource objects. Options are represented as key-value pairs
 * and individually passed to the handler.
 *
 * @author jmoringe
 */
class RSC_EXPORT OptionHandler {
public:
    virtual ~OptionHandler();

    /** This method is called once for each individual option
     * available from a given @ref ConfigSource.
     *
     * @param key A list of hierarchical key components that identify
     * the option.
     * @param value The value of the option.
     */
    virtual void handleOption(const std::vector<std::string>& key,
                              const std::string& value) = 0;
};

}
}
