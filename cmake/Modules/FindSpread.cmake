# - Find the Spread Toolkit
# This module defines:
#  SPREAD_INCLUDE_DIRS, where to find sp.h, etc.
#  SPREAD_LIBRARIES, the libraries needed to use spread.
#  SPREAD_EXECUTABLE, the spread daemon executable.
#
# Will react on:
#  SPREAD_ROOT, additional directory to search for spread
#  SPREAD_USE_STATIC, if enabled, search for static lib instead of shared, default OFF
#  SPREAD_IGNORE_EXECUTABLE, if set to true, a missing spread executable will be ignored, default OFF 
#
# Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The development of this software was supported by:
#   CoR-Lab, Research Institute for Cognition and Robotics
#     Bielefeld University

INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(SPREAD_INCLUDE_DIRS
          NAMES sp.h
          PATHS "${SPREAD_ROOT}/include"
                "$ENV{HOME}/include")

FIND_PROGRAM(SPREAD_EXECUTABLE
             NAMES spread
             PATHS "${SPREAD_ROOT}/sbin"
                   "${SPREAD_ROOT}/bin"
                   "sbin")

SET(SPREAD_LIBNAME "libspread${CMAKE_SHARED_LIBRARY_SUFFIX}")
IF(SPREAD_USE_STATIC OR MSVC)
    # the dfeault spread project for MSVC generates a static lib
    SET(SPREAD_LIBNAME "libspread${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

MESSAGE(STATUS "Searching for spread library with name ${SPREAD_LIBNAME}")

FIND_LIBRARY(SPREAD_LIBRARIES
             NAMES ${SPREAD_LIBNAME}
             PATHS "${SPREAD_ROOT}/lib"
                   "${SPREAD_ROOT}/bin"
                   "$ENV{HOME}/lib")

IF(SPREAD_IGNORE_EXECUTABLE)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Spread DEFAULT_MSG SPREAD_LIBRARIES SPREAD_INCLUDE_DIRS)
ELSE()
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Spread DEFAULT_MSG SPREAD_LIBRARIES SPREAD_INCLUDE_DIRS SPREAD_EXECUTABLE)
ENDIF()

MARK_AS_ADVANCED(SPREAD_INCLUDE_DIRS SPREAD_LIBNAME SPREAD_LIBRARIES SPREAD_EXECUTABLE)
