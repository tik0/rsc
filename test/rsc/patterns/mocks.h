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

#include <stdexcept>
#include <string>

struct Interface {
    virtual ~Interface() {
    }
};

struct Impl1: Interface {
    std::string stringMember;
    float floatMember;

    Impl1(const std::string& stringParam, float floatParam) :
        stringMember(stringParam), floatMember(floatParam) {
    }

    static Interface*
    create(const rsc::runtime::Properties& props) {
        return new Impl1(props.get<std::string> ("string_param"), props.get<
                float> ("float_param"));
    }
};

struct Impl2: Interface {
    Impl2() {
    }

    static Interface*
    create(const rsc::runtime::Properties&) {
        return new Impl2();
    }
};

struct ImplFailingConstructor: Interface {
    ImplFailingConstructor() {
        throw std::runtime_error("intentionally failing constructor");
    }

    static Interface*
    create(const rsc::runtime::Properties&) {
        return new ImplFailingConstructor();
    }
};
