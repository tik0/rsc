# - Try to find Eigen2 lib
#
# This module supports requiring a minimum version, e.g. you can do
#   find_package(Eigen2 2.0.3)
# to require version 2.0.3 to newer of Eigen2.
#
# Once done this will define
#
#  EIGEN2_FOUND - system has eigen lib with correct version
#  EIGEN2_INCLUDE_DIR - the eigen include directory
#  EIGEN2_VERSION - eigen version

# Copyright (c) 2006, 2007 Montel Laurent, <montel@kde.org>
# Copyright (c) 2008, 2009 Gael Guennebaud, <g.gael@free.fr>
# Redistribution and use is allowed according to the terms of the BSD license.

IF(NOT Eigen2_FIND_VERSION)
    IF(NOT Eigen2_FIND_VERSION_MAJOR)
        SET(Eigen2_FIND_VERSION_MAJOR 2)
    ENDIF()
    IF(NOT Eigen2_FIND_VERSION_MINOR)
        SET(Eigen2_FIND_VERSION_MINOR 0)
    ENDIF()
    IF(NOT Eigen2_FIND_VERSION_PATCH)
        SET(Eigen2_FIND_VERSION_PATCH 0)
    ENDIF()

    SET(Eigen2_FIND_VERSION "${Eigen2_FIND_VERSION_MAJOR}.${Eigen2_FIND_VERSION_MINOR}.${Eigen2_FIND_VERSION_PATCH}")
ENDIF()

MACRO(_eigen2_check_version)
    FILE(READ "${EIGEN2_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h" _eigen2_version_header LIMIT 5000 OFFSET 1000)

    STRING(REGEX MATCH "define *EIGEN_WORLD_VERSION ([0-9]*)" _eigen2_world_version_match "${_eigen2_version_header}")
    SET(EIGEN2_WORLD_VERSION "${CMAKE_MATCH_1}")
    STRING(REGEX MATCH "define *EIGEN_MAJOR_VERSION ([0-9]*)" _eigen2_major_version_match "${_eigen2_version_header}")
    SET(EIGEN2_MAJOR_VERSION "${CMAKE_MATCH_1}")
    STRING(REGEX MATCH "define *EIGEN_MINOR_VERSION ([0-9]*)" _eigen2_minor_version_match "${_eigen2_version_header}")
    SET(EIGEN2_MINOR_VERSION "${CMAKE_MATCH_1}")

    SET(EIGEN2_VERSION ${EIGEN2_WORLD_VERSION}.${EIGEN2_MAJOR_VERSION}.${EIGEN2_MINOR_VERSION})
    IF(${EIGEN2_VERSION} VERSION_LESS ${Eigen2_FIND_VERSION})
        SET(EIGEN2_VERSION_OK FALSE)
    ELSE()
        SET(EIGEN2_VERSION_OK TRUE)
    ENDIF()

    IF(NOT EIGEN2_VERSION_OK)
        MESSAGE(SEND_ERROR "Eigen2 version ${EIGEN2_VERSION} found in ${EIGEN2_INCLUDE_DIR}, "
                       "but at least version ${Eigen2_FIND_VERSION} is required")
    ENDIF()
ENDMACRO()

IF(EIGEN2_INCLUDE_DIR)
    # in cache already
    _eigen2_check_version()
    SET(EIGEN2_FOUND ${EIGEN2_VERSION_OK})
ELSE()

    FIND_PATH(EIGEN2_INCLUDE_DIR NAMES Eigen/Core
         PATHS
         ${INCLUDE_INSTALL_DIR}
         "C:/Program Files/Eigen/include"
         PATH_SUFFIXES eigen2)

    IF(EIGEN2_INCLUDE_DIR)
        _eigen2_check_version()
    ENDIF()

    INCLUDE(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Eigen2 DEFAULT_MSG EIGEN2_INCLUDE_DIR EIGEN2_VERSION_OK)

    MARK_AS_ADVANCED(EIGEN2_INCLUDE_DIR)

ENDIF(EIGEN2_INCLUDE_DIR)

