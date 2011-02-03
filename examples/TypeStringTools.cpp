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

#include <iostream>

#include <rsc/runtime/TypeStringTools.h>

struct my_base {
  virtual void
  f() = 0;
};

struct my_derived : my_base {
  void
  f() {
  }
};

int
main(int, char*[]) {
  std::cout << rsc::runtime::type_name<int>() << std::endl;

  int i = 1;
  std::cout << rsc::runtime::type_name(i) << std::endl;

  my_base* base = new my_derived();
  std::cout << rsc::runtime::type_name(*base) << std::endl;

  return EXIT_SUCCESS;
}
