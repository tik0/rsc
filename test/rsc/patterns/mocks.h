/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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
