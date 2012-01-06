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

#include <iostream>

#include <rsc/runtime/TypeStringTools.h>

struct my_base {
    virtual ~my_base() {
    }
    virtual void
    f() = 0;
};

struct my_derived: my_base {
    void f() {
    }
};

int main(int, char*[]) {
    std::cout << rsc::runtime::typeName<int>() << std::endl;

    int i = 1;
    std::cout << rsc::runtime::typeName(i) << std::endl;

    my_base* base = new my_derived();
    std::cout << rsc::runtime::typeName(*base) << std::endl;
    delete base;

    return EXIT_SUCCESS;
}
