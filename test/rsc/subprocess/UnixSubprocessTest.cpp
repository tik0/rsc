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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iomanip>

#include <boost/filesystem.hpp>

#include "rsc/subprocess/UnixSubprocess.h"
#include "rsc/misc/langutils.h"
#include "testconfig.h"

using namespace std;
using namespace testing;
using namespace rsc;
using namespace rsc::misc;
using namespace rsc::subprocess;

TEST(UnixSubprocessTest, testSubprocess)
{

    srand(time(NULL));

    const string unique = randAlnumStr(10);
    const string startedName = "/tmp/started-" + unique;
    const string killedName = "/tmp/killed-" + unique;

    vector<string> args;
    args.push_back("-c");
    args.push_back("\"" + TEST_ROOT + "/rsc/subprocess/killNotify\" \""
            + startedName + "\" \"" + killedName + "\"");
    {
        UnixSubprocess subprocess("/bin/bash", args);
        // wait for the process to start
        cout << "Waiting for strt file" << endl;
        do {
            // TODO timeouts
            usleep(50000);
        } while (!boost::filesystem::exists(startedName));
    }

    cout << "Waiting for kill file" << endl;
    do {
        // TODO timeouts
        usleep(50000);
    } while (!boost::filesystem::exists(killedName));

    boost::filesystem::remove(startedName);
    boost::filesystem::remove(killedName);

}
