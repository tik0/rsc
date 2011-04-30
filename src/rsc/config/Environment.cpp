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

#include "Environment.h"

#include <algorithm>
#include <iterator>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/environment_iterator.hpp>

#include <rsc/runtime/ContainerIO.h>

using namespace std;

using namespace boost;
using namespace boost::filesystem;

using namespace rsc::logging;

namespace rsc {
namespace config {

path userHomeDirectory() {
#ifndef WIN32
    string home = getenv("HOME") + string("/");
#else
    string home = string(getenv("HOMEDRIVE")) + string(getenv("HOMEPATH")) + string("\\");
#endif
    return home;
}

path userConfigDirectory() {
    return userHomeDirectory() / ".config";
}

string transformName(const string &name) {
    if ((name.size() >= 4) && (name.substr(0, 4) == "RSB_")) {
        string result;
        transform(name.begin() + 4, name.end(), back_inserter(result), &::tolower);
        return result;
    } else {
        return "";
    }
}

EnvironmentVariableSource::EnvironmentVariableSource() :
    logger(Logger::getLogger("rsc.config.EnvironmentVariableSource")) {
}

void EnvironmentVariableSource::emit(OptionHandler &handler) {
    for (environment_iterator it = environment_iterator(environ);
        it != environment_iterator(); ++it) {
        string name = transformName(it->first);
        if (name.empty())
            continue;

        vector<string> key;
        split(key, name, is_any_of("_"));
        string value = it->second;

        RSCTRACE(logger, "Option " << key << " -> " << value);

        handler.handleOption(key, value);
    }
}

}
}
