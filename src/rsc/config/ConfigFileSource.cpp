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

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "../runtime/ContainerIO.h"

using namespace std;

using namespace boost;

using namespace rsc::logging;

namespace rsc {
namespace config {

// Taken from Boost.ProgramOptions
string trim(const string& s) {
    string::size_type n, n2;
    n = s.find_first_not_of(" \t\r\n");
    if (n == string::npos) {
        return string();
    } else {
        n2 = s.find_last_not_of(" \t\r\n");
        return s.substr(n, n2-n+1);
    }
}

ConfigFileSource::ConfigFileSource(istream &stream) :
    logger(Logger::getLogger("rsc.config.ConfigFileSource")),
    stream(stream) {
}

void ConfigFileSource::emit(OptionHandler& handler) {
    string name;
    string value;
    while (getOption(name, value)) {

        vector<string> key;
        split(key, name, is_any_of("."));

        RSCTRACE(logger, "Option " << key << " -> " << value);

        handler.handleOption(key, value);
    }
}

// Based on Boost.ProgramOptions
bool ConfigFileSource::getOption(string& name, string &value) {
    string line;
    while (getline(this->stream, line)) {
        // strip '#' comments and whitespace
        string::size_type n;
        if ((n = line.find('#')) != string::npos)
            line = line.substr(0, n);
        line = trim(line);

        if (!line.empty()) {
            // Handle section name
            if (*line.begin() == '[' && *line.rbegin() == ']') {
                currentSection = line.substr(1, line.size()-2);
                if (*currentSection.rbegin() != '.') {
                    currentSection += '.';
                }
            } else if ((n = line.find('=')) != string::npos) {
                name = currentSection + trim(line.substr(0, n));
                value = trim(line.substr(n+1));
                return true;
            } else {
                throw invalid_argument(str(format("Syntax error in line `%1%'")
                                           % line));
            }
        }
    }
    return false;
  }


}
}
