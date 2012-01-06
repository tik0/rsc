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

#include "rsc/patterns/Factory.h"

struct MyInterface {
    virtual ~MyInterface() {
    }
    virtual void
    doStuff() = 0;
};

struct MyImpl1: MyInterface {
    MyImpl1(const std::string& myStringParam, float myFloatParam) {
        std::cout << "my_impl_1: " << myStringParam << " " << myFloatParam
                << std::endl;
    }

    void doStuff() {
        std::cout << "MyImpl1: doing stuff" << std::endl;
    }

    static MyInterface*
    create(const rsc::runtime::Properties& props) {
        return new MyImpl1(props.get<std::string> ("my_string_param"),
                props.get<float> ("my_float_param"));
    }
};

struct MyImpl2: MyInterface {
    MyImpl2(int myIntParam, float myFloatParam) {
        std::cout << "MyImpl2: " << myIntParam << " " << myFloatParam
                << std::endl;
    }

    void doStuff() {
        std::cout << "myImpl2: doing stuff" << std::endl;
    }

    static MyInterface*
    create(const rsc::runtime::Properties& props) {
        return new MyImpl2(props.get<int> ("my_int_param"),
                props.get<float> ("my_float_param"));
    }
};

typedef rsc::patterns::SingletonFactory<std::string, MyInterface> MyFactory;

void registerImpls() {
    MyFactory& factory = MyFactory::getInstance();
    factory.impls().register_("my_impl_1", &MyImpl1::create);
    factory.impls().register_("my_impl_2", &MyImpl2::create);

    std::cout << "registered " << factory.impls().size() << " impls:"
            << std::endl << factory << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: factory [IMPL] [PARAM]" << std::endl;
        return EXIT_FAILURE;
    }

    registerImpls();

    MyFactory& factory = MyFactory::getInstance();

    rsc::runtime::Properties props;
    if (argc >= 3)
        props["my_string_param"] = static_cast<std::string> (argv[2]);
    props["my_int_param"] = 1;
    //props["my_float_param"] = 1.0; // this is a double; does not work as float param
    props["my_float_param"] = static_cast<float> (1.0);

    try {
        if (std::string(argv[1]) != "all") {
            std::cout << "using impl " << argv[1] << std::endl;

            MyInterface* inst = factory.createInst(argv[1], props);

            inst->doStuff();
        } else {
            std::cout << "using all impls" << std::endl;

            for (MyFactory::ImplMap::iterator it = factory.impls().begin(); it
                    != factory.impls().end(); ++it) {
	        MyInterface* inst = it->second(props);
                inst->doStuff();
            }
        }
    } catch (const rsc::patterns::NoSuchImpl&) {
        std::cerr << "no such implementation: " << argv[1] << std::endl;

        // Print registered implementations
        std::cout << factory << std::endl;
    } catch (const rsc::patterns::ConstructError& e) {
        std::cerr << "construct error: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
