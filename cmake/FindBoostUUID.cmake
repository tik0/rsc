# -*- mode: cmake -*-

# - Try to find the boost uuid library
# Will define:
#
# BOOSTUUID_INCLUDE_DIRS - include directories needed to use the library
#
# Uses:
#
# BOOSTUUID_ROOT - hint for the installation root of boost uuid
#
# Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
#
# This program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation;
# either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(BOOSTUUID_INCLUDE_DIRS
          NAMES boost/uuid/uuid.hpp
          HINTS ${BOOSTUUID_ROOT} ${BOOST_ROOT} ${CMAKE_INSTALL_PREFIX}
          PATH_SUFFIXES include)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(BoostUUID DEFAULT_MSG BOOSTUUID_INCLUDE_DIRS)

# only visible in advanced view
MARK_AS_ADVANCED(BOOSTUUID_INCLUDE_DIRS)
