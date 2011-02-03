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

#include <utility>
#include <list>
#include <vector>
#include <set>
#include <map>

#include <iostream>

#include <rsc/runtime/ContainerIO.h>

int
main(int, char*[]) {
  // Pair example
  std::pair<double, std::string> pair(5.0, "a");
  std::cout << "a pair:   " << pair << std::endl;

  std::cout << "a pair:   " << std::pair_whitespace << pair << std::endl;

  std::detail::set_pair_style<std::detail::pair_style> my_style = {
    std::detail::pair_style("[", ":", "]")
  };
  std::cout << "a pair:   " << my_style << pair << std::endl;

  // List example
  std::list<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);

  std::cout << "a list:   " << list << std::endl;

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

  return EXIT_SUCCESS;
}
