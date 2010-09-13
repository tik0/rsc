/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "rsc/logging/ConsoleLogger.h"

#include <iostream>

#include "rsc/misc/langutils.h"

using namespace std;

namespace rsc {
namespace logging {

ConsoleLogger::ConsoleLogger(const string &name) :
    name(name), level(INFO) {

}

ConsoleLogger::ConsoleLogger(const string &name, const Level &level) :
    name(name), level(level) {
}

ConsoleLogger::~ConsoleLogger() {
}

Logger::Level ConsoleLogger::getLevel() const {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return level;
}

void ConsoleLogger::setLevel(const Logger::Level &level) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    this->level = level;
}

string ConsoleLogger::getName() const {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return name;
}

void ConsoleLogger::setName(const string &name) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    this->name = name;
}

ostream &ConsoleLogger::printHeader(ostream &stream, const Level &level) {
    return stream << rsc::misc::currentTimeMillis() << " " << this->name
            << " [" << level << "]: ";
}

void ConsoleLogger::log(const Level &level, const string &msg) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    if (isEnabledFor(level)) {
        if (level <= Logger::WARN) {
            printHeader(cerr, level);
            cerr << msg << endl;
        } else {
            printHeader(cout, level);
            cout << msg << endl;
        }
    }
}

}
}
