/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2011 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
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

#include <sstream>

#include <boost/shared_ptr.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/runtime/Printable.h"
#include "rsc/misc/langutils.h"

using namespace std;
using namespace rsc::runtime;
using namespace testing;

class DummyPrintable: public Printable {
public:

    DummyPrintable(const string &name) :
        name(name) {
    }

    string getClassName() const {
        return name;
    }

    void printContents(ostream &stream) const {
        stream << "CONTENTS";
    }

private:
    string name;

};

TEST(PrintableTest, testPrint)
{

    const string name = "AName";
    DummyPrintable p(name);
    {
        stringstream s;
        p.print(s);
        EXPECT_EQ(name + "[CONTENTS]", s.str());
    }

    {
        stringstream s;
        s << p;
        EXPECT_EQ(name + "[CONTENTS]", s.str());
    }

    {
        stringstream s;
        boost::shared_ptr<DummyPrintable> ptr(&p, rsc::misc::NullDeleter());
        s << ptr;
        EXPECT_EQ("*" + name + "[CONTENTS] at ", s.str().substr(0, 20));
    }

    {
        stringstream s;
        boost::shared_ptr<Printable> ptr(&p, rsc::misc::NullDeleter());
        s << ptr;
        EXPECT_EQ("*" + name + "[CONTENTS] at ", s.str().substr(0, 20));
    }

}
