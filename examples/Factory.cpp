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

#include "rsc/patterns/Factory.h"

struct my_interface {
  virtual void
  do_stuff() = 0;
};

struct my_impl_1 : my_interface {
  my_impl_1(const std::string& my_string_param,
	    float              my_float_param) {
    std::cout << "my_impl_1: "
	      << my_string_param << " "
	      << my_float_param << std::endl;
  }

  void
  do_stuff() {
    std::cout << "my_impl_1: doing stuff" << std::endl;
  }

  static my_interface*
  create(const rsc::runtime::Properties& props) {
    return new my_impl_1(props.get<std::string>("my_string_param"),
			 props.get<float>      ("my_float_param"));
  }
};

struct my_impl_2 : my_interface {
  my_impl_2(int   my_int_param,
	    float my_float_param) {
    std::cout << "my_impl_2: "
	      << my_int_param << " "
	      << my_float_param << std::endl;
  }

  void
  do_stuff() {
    std::cout << "my_impl_2: doing stuff" << std::endl;
  }

  static my_interface*
  create(const rsc::runtime::Properties& props) {
    return new my_impl_2(props.get<int>  ("my_int_param"),
			 props.get<float>("my_float_param"));
  }
};


typedef rsc::patterns::SingletonFactory<std::string, my_interface> my_factory;

void
register_impls() {
  my_factory& factory = my_factory::getInstance();
  factory.impls().register_("my_impl_1", &my_impl_1::create);
  factory.impls().register_("my_impl_2", &my_impl_2::create);

  std::cout << "registered " << factory.impls().size()
	    << " impls:" << std::endl << factory << std::endl;
}


int
main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "usage: factory [IMPL] [PARAM]" << std::endl;
    return EXIT_FAILURE;
  }

  register_impls();

  my_factory& factory = my_factory::getInstance();

  rsc::runtime::Properties props;
  if (argc >= 3)
    props["my_string_param"] = static_cast<std::string>(argv[2]);
  props["my_int_param"]	    = 1;
  //props["my_float_param"] = 1.0; // this is a double; does not work as float param
  props["my_float_param"]   = static_cast<float>(1.0);

  try {
    if (std::string(argv[1]) != "all") {
      std::cout << "using impl " << argv[1] << std::endl;

      my_interface* inst = factory.create_inst(argv[1], props);

      inst->do_stuff();
    } else {
      std::cout << "using all impls" << std::endl;

      for (my_factory::impl_map::iterator it = factory.impls().begin();
	   it != factory.impls().end(); ++it) {
	my_interface* inst = it->second(props);
	inst->do_stuff();
      }
    }
  } catch (const rsc::patterns::NoSuchImpl&) {
    std::cerr <<"no such implementation: " << argv[1] << std::endl;

    // Print registered implementations
    std::cout << factory << std::endl;
  } catch (const rsc::patterns::ConstructError& e) {
    std::cerr <<"construct error: " << e.what() << std::endl;
 }

  return EXIT_SUCCESS;
}
