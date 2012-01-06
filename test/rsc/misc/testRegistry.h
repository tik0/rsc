/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
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
    TestRegistree(const std::string& key);
    std::string getRegistryKey() const;
};

REGISTRY_EXPORT rsc::misc::Registry<TestRegistree>* globalTestRegistry();
