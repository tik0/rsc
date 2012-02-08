# - If included before targets, sets compiler warning leves to a super pedantic
#   level.
#
# This only works if included after the line containing the PROJECT call in
# CMake.
#
# Copyright (C) 2011 by Arne Nordmann <jwienke at techfak dot uni-bielefeld dot de>
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

INCLUDE(CheckCXXCompilerFlag)

# TODO find a flag that better tells if this is GCC
IF(UNIX)

    CHECK_CXX_COMPILER_FLAG(-pipe CHECK_CXX_FLAG_pipe)
    IF(CHECK_CXX_FLAG_pipe)
        ADD_DEFINITIONS(-pipe)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wall CHECK_CXX_FLAG_Wall)
    IF(CHECK_CXX_FLAG_Wall)
        ADD_DEFINITIONS(-Wall)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wextra CHECK_CXX_FLAG_Wextra)
    IF(CHECK_CXX_FLAG_Wextra)
        ADD_DEFINITIONS(-Wextra)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-fdiagnostics-show-option CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-fdiagnostics-show-option)
    ENDIF()
    
    
    CHECK_CXX_COMPILER_FLAG(-pedantic CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-pedantic)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wchar-subscripts CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wchar-subscripts)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Winit-self CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Winit-self)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wswitch-enum CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wswitch-enum)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wstrict-aliasing=3 CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wstrict-aliasing=3)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wstrict-overflow=5 CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wstrict-overflow=5)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wundef CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wundef)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wno-endif-labels CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wno-endif-labels)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wshadow CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wshadow)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wno-system-headers CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wno-system-headers)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wno-builtin-macro-redefined CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wno-builtin-macro-redefined)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wmissing-declarations CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wmissing-declarations)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wsign-conversion CHECK_CXX_FLAG_DIAGNOSTICS)
    IF(CHECK_CXX_FLAG_DIAGNOSTICS)
        ADD_DEFINITIONS(-Wsign-conversion)
    ENDIF()
    
    # We want this, but it`s not usable with gtest (loads of warnings from gtest)
    #CHECK_CXX_COMPILER_FLAG(-Wswitch-default CHECK_CXX_FLAG_DIAGNOSTICS)
    #IF(CHECK_CXX_FLAG_DIAGNOSTICS)
    #    ADD_DEFINITIONS(-Wswitch-default)
    #ENDIF()   
    
    # if you are insane, enable this...
    CHECK_CXX_COMPILER_FLAG(-Weffc++ CHECK_CXX_FLAG_EFFCPP)
    IF(CHECK_CXX_FLAG_EFFCPP)
        ADD_DEFINITIONS(-Weffc++)
    ENDIF()

ELSEIF(MSVC)

    # right now W3 is as pedantic as GCC with the above settings
    # still we do not want everything from the compiler...
    
    # needs to have dll-interface to be used by clients
    ADD_DEFINITIONS(/wd4251)
    # exported class derived from non-exported class
    ADD_DEFINITIONS(/wd4275)
    # do not warn for "unsafe" stl functions
    ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)

ENDIF()
