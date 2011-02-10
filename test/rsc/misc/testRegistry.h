/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#pragma once

#include <string>

#include "rsc/misc/Registry.h"

#if defined (_WIN32) 
    #if defined(registry_EXPORTS)
        #define REGISTRY_EXPORT __declspec(dllexport)
        #define REGISTRY_EXPIMP
    #else
        #define REGISTRY_EXPORT __declspec(dllimport)
        #define REGISTRY_EXPIMP extern
    #endif
#else
    #define REGISTRY_EXPORT
#endif

class REGISTRY_EXPORT TestRegistree {
public:
    std::string key;
    TestRegistree(const std::string &key);
    std::string getRegistryKey() const;
};

REGISTRY_EXPORT rsc::misc::Registry<TestRegistree> *globalTestRegistry();
