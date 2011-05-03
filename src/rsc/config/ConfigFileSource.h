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

#include <iostream>

#include "../logging/Logger.h"

#include "ConfigSource.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/** Objects of this class parse streams that contain configuration
 * information in "ini-file" syntax. Sections and keys are mapped to
 * hierarchical names.
 *
 * @author jmoringe
 */
class RSC_EXPORT ConfigFileSource : public ConfigSource{
public:
    ConfigFileSource(std::istream &stream);

    void emit(OptionHandler& handler);
private:
    logging::LoggerPtr logger;

    std::istream& stream;
    std::string currentSection;

    bool getOption(std::string& name, std::string &value);
};

}
}
