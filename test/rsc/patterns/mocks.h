/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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

#include <stdexcept>
#include <string>

struct interface {
  virtual ~interface() {};
};

struct impl_1 : interface {
  std::string string_member;
  float	      float_member;

  impl_1(const std::string& string_param,
	 float              float_param)
   : string_member(string_param),
     float_member(float_param) {
  }

  static interface*
  create(const rsc::runtime::Properties& props) {
    return new impl_1(props.get<std::string>("string_param"),
		      props.get<float>      ("float_param"));
  }
};

struct impl_2 : interface {
  impl_2() {
  }

  static interface*
  create(const rsc::runtime::Properties&) {
    return new impl_2();
  }
};

struct impl_failing_constructor : interface {
  impl_failing_constructor() {
    throw std::runtime_error("intentionally failing constructor");
  }

  static interface*
  create(const rsc::runtime::Properties&) {
    return new impl_failing_constructor();
  }
};
