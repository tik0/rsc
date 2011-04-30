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

#include "../runtime/Properties.h"

#include "OptionHandler.h"

namespace rsc {
namespace config {

/** This implementation of the @ref OptionHandler interface stores all
 * options it receives into a @ref runtime::Properties object.
 *
 * @author jmoringe
 */
class PropertyHandler : public OptionHandler {
public:
    void handleOption(const std::vector<std::string> &key,
                      const std::string &value);
    /** Returns the @ref runtime::Properties object constructed by
     * this handler.
     * 
     * @return A constant reference to the properties.
     */
    const runtime::Properties& getProperties() const;
private:
    runtime::Properties result;
};

}
}
