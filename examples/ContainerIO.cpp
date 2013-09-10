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

#include <utility>
#include <list>
#include <vector>
#include <set>
#include <map>

#include <iostream>

#include <rsc/runtime/ContainerIO.h>

int main(int, char*[]) {
    // Pair example
    std::pair<double, std::string> pair(5.0, "a");
    std::cout << "a pair:   " << pair << std::endl;

    std::cout << "a pair:   " << std::pair_whitespace << pair << std::endl;

    std::detail::set_pair_style<std::detail::pair_style> my_style = {
            std::detail::pair_style("[", ":", "]") };
    std::cout << "a pair:   " << my_style << pair << std::endl;

    // List example
    std::list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    std::cout << "a list:   " << list << std::endl;

    std::cout << "a list:   " << std::container_multiline << list << std::endl;
    std::cout << std::container_singleline;

    // Vector example
    std::vector<int> vector;
    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);

    std::cout << "a vector: " << vector << std::endl;

    // Set example
    std::set<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    set.insert(4);

    std::cout << "a set:    " << set << std::endl;

    // Map example
    std::map<std::string, double> map;
    map.insert(std::make_pair("a", 1.0));
    map.insert(std::make_pair("b", 2.0));
    map.insert(std::make_pair("c", 3.0));

    std::cout << "a map:    " << map << std::endl;
    std::cout << "a map:    " << std::pair_default << std::container_multiline
            << map << std::endl;

    return EXIT_SUCCESS;
}
