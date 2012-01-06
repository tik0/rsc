/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
 *
 * This file may be licensed under the terms of the
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

#include <string>
#include <iostream>

#include "rsc/runtime/Properties.h"

int main(int, char*[]) {
    rsc::runtime::Properties props;
    std::cout << "empty:       " << props << std::endl;

    props["key1"] = 1;
    props["key2"] = 2u;
    props["key3"] = 3.0;
    props["key4"] = std::string("four");
    std::cout << "some values: " << props << std::endl;

    // Forcing entry type
    props.set<unsigned int> ("uint", 1.0);
    std::cout << "forced type: " << props << std::endl;

    // Lookup
    std::cout << "key1:        " << props.get<int> ("key1") << std::endl;

    std::cout << "key2:        " << boost::any_cast<unsigned int>(props["key2"])
            << std::endl;

    try {
        std::cout << "key2:        " << boost::any_cast<std::string>(props["key2"]) // wrong type
                << std::endl;
    } catch (const boost::bad_any_cast& e) {
        std::cerr << "error:       " << e.what() << std::endl;
    }

    //
    try {
        std::cout << "invalid:     " << props.get<bool> ("invalid") << std::endl;
    } catch (const rsc::runtime::NoSuchObject& e) {
        std::cerr << "error:       " << e.what() << std::endl;
    }

    std::cout << "invalid:     " << props.get<bool> ("invalid", true) << std::endl; // default

    // Merging of properties
    rsc::runtime::Properties props2;
    props2["key1"]   = 42;
    props2["unique"] = std::string("only in props2");

    std::cout << "merged:      " << (props << props2) << std::endl;

    return EXIT_SUCCESS;
}
