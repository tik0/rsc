# - Try to find Controller Basis Framework 
# Will define:
#
# CBF_FOUND - If CBF was found
# CBF_INCLUDE_DIRS - Include directories
# CBF_LIBRARIES - CBF library
# CBF_DEFINES - CBF compiler defines
#
# Possible hints:
# CBF_ROOT - root directory of the CBF innstallation
#
# Copyright (C) 2012 by Arne Nordmann <anordman at cor-lab dot de>
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

IF(UNIX)

    # Try to find cbf.h
    FIND_PATH(CBF_DIR
              NAMES cbf/cbf.h
              HINTS "${CBF_ROOT}"
                    "${CBF_ROOT}/include"
                    "${CMAKE_INSTALL_PREFIX}"
                    "${CMAKE_INSTALL_PREFIX}/include")

    IF(CBF_DIR)
    
        SET(CBF_FOUND TRUE)
        SET(CBF_INCLUDE_DIRS ${CBF_DIR})        

        FIND_LIBRARY(CBF_LIBRARIES cbf
              HINTS "${CBF_DIR}/../lib"
                    "${CBF_ROOT}/../lib"
                    "${CBF_ROOT}/lib"
                    "${CMAKE_INSTALL_PREFIX}/lib")

        SET(CBF_DEFINES "-g -ggdb3 -O2")

    ENDIF(CBF_DIR)

ENDIF(UNIX)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CBF DEFAULT_MSG CBF_FOUND CBF_DIR)

