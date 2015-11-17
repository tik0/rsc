# - Try to find Eigen3 lib
#
# This module supports requiring a minimum version, e.g. you can do
#   find_package(Eigen3 3.0.3)
# to require version 3.0.3 to newer of Eigen3.
#
# Once done this will define
#
#  EIGEN3_FOUND - system has eigen lib with correct version
#  EIGEN3_INCLUDE_DIR - the eigen include directory
#  EIGEN3_VERSION - eigen version

# Copyright (c) 2006, 2007 Montel Laurent, <montel@kde.org>
# Copyright (c) 2008, 2009 Gael Guennebaud, <g.gael@free.fr>
# Copyright (c) 2012, Arne Nordmann, <anordman@cor-lab.de>
# Redistribution and use is allowed according to the terms of the BSD license.

IF(NOT Eigen3_FIND_VERSION)
    IF(NOT Eigen3_FIND_VERSION_MAJOR)
        SET(Eigen3_FIND_VERSION_MAJOR 3)
    ENDIF()
    IF(NOT Eigen3_FIND_VERSION_MINOR)
        SET(Eigen3_FIND_VERSION_MINOR 0)
    ENDIF()
    IF(NOT Eigen3_FIND_VERSION_PATCH)
        SET(Eigen3_FIND_VERSION_PATCH 0)
    ENDIF()

    SET(Eigen3_FIND_VERSION
        "${Eigen3_FIND_VERSION_MAJOR}.${Eigen3_FIND_VERSION_MINOR}.${Eigen3_FIND_VERSION_PATCH}")
ENDIF()

MACRO(_eigen3_check_version)
    FILE(READ "${EIGEN3_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h"
        _eigen3_version_header LIMIT 5000)

    STRING(REGEX MATCH "define *EIGEN_WORLD_VERSION ([0-9]*)" _eigen3_world_version_match "${_eigen3_version_header}")
    SET(EIGEN3_WORLD_VERSION "${CMAKE_MATCH_1}")
    STRING(REGEX MATCH "define *EIGEN_MAJOR_VERSION ([0-9]*)" _eigen3_major_version_match "${_eigen3_version_header}")
    SET(EIGEN3_MAJOR_VERSION "${CMAKE_MATCH_1}")
    STRING(REGEX MATCH "define *EIGEN_MINOR_VERSION ([0-9]*)" _eigen3_minor_version_match "${_eigen3_version_header}")
    SET(EIGEN3_MINOR_VERSION "${CMAKE_MATCH_1}")

    SET(EIGEN3_VERSION ${EIGEN3_WORLD_VERSION}.${EIGEN3_MAJOR_VERSION}.${EIGEN3_MINOR_VERSION})
    IF(${EIGEN3_VERSION} VERSION_LESS ${Eigen3_FIND_VERSION})
        SET(EIGEN3_VERSION_OK FALSE)
    ELSE()
        SET(EIGEN3_VERSION_OK TRUE)
    ENDIF()

    IF(NOT EIGEN3_VERSION_OK)
        MESSAGE(SEND_ERROR "Eigen3 version ${EIGEN3_VERSION} found in ${EIGEN3_INCLUDE_DIR}, "
                       "but at least version ${Eigen3_FIND_VERSION} is required")
    ENDIF()
ENDMACRO()

IF(EIGEN3_INCLUDE_DIR)
    # in cache already
    _eigen3_check_version()
    SET(EIGEN3_FOUND ${EIGEN3_VERSION_OK})
ELSE()

    FIND_PATH(EIGEN3_INCLUDE_DIR NAMES Eigen/Core
         PATHS
         ${INCLUDE_INSTALL_DIR}
         "C:/Program Files/Eigen/include"
         PATH_SUFFIXES eigen3)

    IF(EIGEN3_INCLUDE_DIR)
        _eigen3_check_version()
    ENDIF()

    INCLUDE(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Eigen3 DEFAULT_MSG EIGEN3_INCLUDE_DIR EIGEN3_VERSION_OK)

    MARK_AS_ADVANCED(EIGEN3_INCLUDE_DIR)

ENDIF()
