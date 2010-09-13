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

#include "InterruptedException.h"

using namespace std;

namespace rsc {
namespace threading {

InterruptedException::InterruptedException() :
    runtime_error("interrupted") {
}

InterruptedException::InterruptedException(const string &message) :
    runtime_error(message) {
}

InterruptedException::~InterruptedException() throw() {
}

}
}
