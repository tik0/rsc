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

#include "ConfigFileSource.h"

#include <boost/algorithm/string.hpp>
#include <boost/program_options/detail/convert.hpp>
#include <boost/program_options/detail/config_file.hpp>

#include "../runtime/ContainerIO.h"

using namespace std;

using namespace boost;

using namespace rsc::logging;

namespace rsc {
namespace config {

typedef program_options::detail::basic_config_file_iterator<char> config_file_iterator;

ConfigFileSource::ConfigFileSource(istream &stream) :
    logger(Logger::getLogger("rsc.config.ConfigFileSource")),
    stream(stream) {
}

void ConfigFileSource::emit(OptionHandler& handler) {
    for (config_file_iterator it
             = config_file_iterator(stream, set<string>(), true);
         it != config_file_iterator(); ++it) {

        vector<string> key;
        split(key, it->string_key, is_any_of("."));
        string value = it->value[0];

        RSCTRACE(logger, "Option " << key << " -> " << it->value);

        handler.handleOption(key, value);
    }
}

}
}
