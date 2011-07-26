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

// For dynamic libs the environ variable is not available
// on MacOS, hence a workaround is needed. See also:
// http://article.gmane.org/gmane.comp.lib.boost.devel/103843
#if defined(__APPLE__)  && defined(__DYNAMIC__)
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

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
    char *rawHome = getenv("HOME");
    if (!rawHome) {
        throw runtime_error("Home directory not defined in HOME variable.");
    }
    string home = string(rawHome) + string("/");
#else
    char * rawHomeDrive = getenv("HOMEDRIVE");
    if (!rawHomeDrive) {
        throw runtime_error("HOMEDRIVE variable not set.");
    }
    char *rawHomePath = getenv("HOMEPATH");
    if (!rawHomePath) {
        throw runtime_error("HOMEPATH variable not set.");
    }
    string home = string(rawHomeDrive) + string(rawHomePath) + string("\\");
#endif
    return home;
}

path userConfigDirectory() {
    return userHomeDirectory() / ".config";
}

string transformName(const string &name, const string &prefix) {
    if (starts_with(name, prefix)) {
        string result;
        transform(name.begin() + prefix.size(), name.end(), back_inserter(
                result), &::tolower);
        return result;
    } else {
        return "";
    }
}

EnvironmentVariableSource::EnvironmentVariableSource(const string &prefix) :
    logger(Logger::getLogger("rsc.config.EnvironmentVariableSource")), prefix(
            prefix) {
}

void EnvironmentVariableSource::emit(OptionHandler &handler) {
    for (environment_iterator it = environment_iterator(environ); it
            != environment_iterator(); ++it) {
        string name = transformName(it->first, this->prefix);
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
