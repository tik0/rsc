# - Generate a doxygen documentation for a project.
# TODO documentation
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

FIND_PACKAGE(Doxygen)

SET(MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})

FUNCTION(GENERATE_DOXYGEN)

    IF(DOXYGEN_FOUND)
        
        # argument parsing
        PARSE_ARGUMENTS(ARG "NAME;VERSION;INSTALL_DIR" "AUTO_INSTALL" ${ARGN})
    
        # name
        SET(DOC_PROJECT_NAME "${CMAKE_PROJECT_NAME}")
        LIST(LENGTH ARG_NAME NAME_LENGTH)
        IF(${NAME_LENGTH} EQUAL 1)
            SET(DOC_PROJECT_NAME "${ARG_NAME}")
        ENDIF()
        
        # version
        SET(DOC_PROJECT_VERSION "")
        LIST(LENGTH ARG_VERSION VERSION_LENGTH)
        IF(${VERSION_LENGTH} EQUAL 1)
            SET(DOC_PROJECT_VERSION "${ARG_VERSION}")
        ENDIF()
        
        # install dir
        SET(DOC_INSTALL_DIR "share/${DOC_PROJECT_NAME}") 
        LIST(LENGTH ARG_INSTALL_DIR INSTALL_DIR_LENGTH)
        IF(${INSTALL_DIR_LENGTH} EQUAL 1)
            SET(DOC_INSTALL_DIR "${ARG_INSTALL_DIR}")
        ENDIF()
        
        SET(DOC_API_DIR ${CMAKE_BINARY_DIR}/doc)
        SET(DOC_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
        SET(DOC_WARN_FILE ${CMAKE_BINARY_DIR}/doxygen-warn.log)
        
        SET(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)
        CONFIGURE_FILE("${MODULE_DIR}/CMakeDoxyfile.in" "${DOXYFILE}")
        
        ADD_CUSTOM_COMMAND(OUTPUT ${DOC_API_DIR}
                           DEPENDS ${DOXYFILE}
                           COMMAND ${DOXYGEN_EXECUTABLE}
                           ARGS ${DOXYFILE}
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        ADD_CUSTOM_TARGET(doc DEPENDS ${DOC_API_DIR})
        
        IF(ARG_AUTO_INSTALL)
            INSTALL(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} doc)")
            INSTALL(DIRECTORY ${DOC_API_DIR} DESTINATION "${DOC_INSTALL_DIR}")
        ENDIF()
        
        MESSAGE(STATUS "Generated doxygen target (doc):
       PROJECT_NAME: ${DOC_PROJECT_NAME}
       PROJECT_VERSION: ${DOC_PROJECT_VERSION}
       INSTALL_DIR: ${DOC_INSTALL_DIR}
       AUTO_INSTALL: ${ARG_AUTO_INSTALL}")
    
    ENDIF()

ENDFUNCTION()
