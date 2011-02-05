/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
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

#include <string>
#include <iostream>

#include "rsc/runtime/Properties.h"

int main(int, char*[]) {
    rsc::runtime::Properties props;
    std::cout << "empty " << props << std::endl;

    props["key1"] = 1;
    props["key2"] = 2u;
    props["key3"] = 3.0;
    props["key4"] = std::string("four");
    std::cout << "some values " << props << std::endl;

    // Forcing entry type
    props.set<unsigned int> ("uint", 1.0);
    std::cout << "forced type " << props << std::endl;

    // Lookup
    std::cout << "key1: " << props.get<int> ("key1") << std::endl;

    std::cout << "key2: " << boost::any_cast<unsigned int>(props["key2"])
            << std::endl;

    try {
        std::cout << "key2: " << boost::any_cast<std::string>(props["key2"]) // wrong type
                << std::endl;
    } catch (const boost::bad_any_cast& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    //
    try {
        std::cout << "invalid: " << props.get<bool> ("invalid") << std::endl;
    } catch (const rsc::runtime::NoSuchObject& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    std::cout << "invalid: " << props.get<bool> ("invalid", true) << std::endl; // default

    return EXIT_SUCCESS;
}
