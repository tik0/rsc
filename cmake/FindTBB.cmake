# - Try to find the Intel Threading Building Blocks library
# Will define:
#
# TBB_INCLUDE_DIRS - include directories needed to use the C++ driver
# TBB_LIBRARIES - Libraries to link agains fpr the driver
# TBB_LIBRARY_DIRS - Directories containing the libraries (win)
#
# Possible hints:
# TBB_ROOT - root directory of the mongodb installation
# TBB_DEBUG - if true, use the debug libraries of tbb instead of stripped ones
# TBB_BITS - value of 32 or 64 indicating the os type, default 32
# TBB_LIB_PATH - path containing the libraries to use
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

FIND_PATH(TBB_INCLUDE_DIRS
          PATHS ${TBB_ROOT}/include
          NAMES tbb.h
          PATH_SUFFIXES tbb)

IF(TBB_DEBUG)
    MESSAGE(STATUS "TBB debug version requested (TBB_DEBUG: ${TBB_DEBUG})")
    SET(TBB_LIB_NAME "tbb_debug")
    SET(TBBMALLOC_LIB_NAME "tbbmalloc_debug")
ELSE()
    MESSAGE(STATUS "TBB normal version requested (TBB_DEBUG: ${TBB_DEBUG})")
    SET(TBB_LIB_NAME "tbb")
    SET(TBBMALLOC_LIB_NAME "tbbmalloc")
ENDIF()

SET(TBB_LIBRARIES "")

# find out if this is a visual studio build and if so which version
STRING(REGEX MATCH "Visual Studio ([0-9]+)" VMATCH ${CMAKE_GENERATOR})
IF(VMATCH)
    STRING(LENGTH ${VMATCH} MATCH_LENGTH)
    STRING(LENGTH "Visual Studio " PREFIX_LENGTH)
    MATH(EXPR VERSION_LENGTH "${MATCH_LENGTH} - ${PREFIX_LENGTH}")
    STRING(SUBSTRING ${VMATCH} ${PREFIX_LENGTH} ${VERSION_LENGTH} VC_VERSION)
    MESSAGE("Visual studio version: ${VC_VERSION}")
ENDIF()

# decide if 32 or 64 bits
IF(NOT TBB_BITS)
    MESSAGE(STATUS "Assuming 32 bit system")
    SET(TBB_BITS "32")
ENDIF()

SET(WIN_LIB_PATH "${TBB_ROOT}/lib/ia${TBB_BITS}/vc${VC_VERSION}")
SET(TBB_LIBRARY_DIRS "${TBB_ROOT}/bin/ia${TBB_BITS}/vc${VC_VERSION}")

FIND_LIBRARY(TBB_LIBRARY
             PATHS ${TBB_ROOT}/lib
                   ${TBB_LIB_PATH}
                   ${WIN_LIB_PATH}
             NAMES ${TBB_LIB_NAME})
FIND_LIBRARY(TBBMALLOC_LIBRARY
             PATHS ${TBB_ROOT}/lib
                   ${TBB_LIB_PATH}
                   ${WIN_LIB_PATH}
             NAMES ${TBBMALLOC_LIB_NAME})
             
IF(TBB_LIBRARY AND TBBMALLOC_LIBRARY)
    SET(TBB_LIBRARIES ${TBB_LIBRARY} ${TBBMALLOC_LIBRARY})
ENDIF()
             
# post-process inlude path
MESSAGE(STATUS "raw TBB include dirs: ${TBB_INCLUDE_DIRS}")
IF(TBB_INCLUDE_DIRS)
    STRING(REGEX REPLACE tbb/?$$ "" TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIRS})
ENDIF()
MESSAGE(STATUS "final TBB include dirs: ${TBB_INCLUDE_DIRS}")
SET(TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIRS} CACHE PATH "TBB include dirs" FORCE)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(TBB DEFAULT_MSG TBB_INCLUDE_DIRS TBB_LIBRARIES)

# only visible in advanced view
MARK_AS_ADVANCED(TBB_INCLUDE_DIRS TBB_LIBRARIES TBB_LIBRARY TBBMALLOC_LIBRARY)
