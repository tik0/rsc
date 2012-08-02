# Locate and configure the Google Protocol Buffers library.
# A modified version of the original macro from CMake 2.8.
# Defines the following variables:
#
#   PROTOBUF_FOUND - Found the Google Protocol Buffers library
#   PROTOBUF_INCLUDE_DIRS - Include directories for Google Protocol Buffers
#   PROTOBUF_LIBRARIES - The protobuf library
#
# The following cache variables are also defined:
#   PROTOBUF_LIBRARY - The protobuf library
#   PROTOBUF_PROTOC_LIBRARY   - The protoc library
#   PROTOBUF_INCLUDE_DIR - The include directory for protocol buffers
#   PROTOBUF_PROTOC_EXECUTABLE - The protoc compiler
#
# These variables are read for additional hints:
#   PROTOBUF_ROOT - Root directory of the protobuf installation if not found
#                   automatically
#
#  ====================================================================
#  Example:
#
#   find_package(ProtocolBuffers REQUIRED)
#   include_directories(${PROTOBUF_INCLUDE_DIRS})
#
#   include_directories(${CMAKE_CURRENT_BINARY_DIR})
#   PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS foo.proto)
#   add_executable(bar bar.cc ${PROTO_SRCS} ${PROTO_HDRS})
#   target_link_libraries(bar ${PROTOBUF_LIBRARY})
#
# NOTE: You may need to link against pthreads, depending
# on the platform.
#  ====================================================================
#
# PROTOBUF_GENERATE_CPP ([CPP srcs hdrs] [JAVA files] [PYTHON files] PROTOFILES files... [PROTOROOT root] [OUTPATH path] [EXPORT_MACRO macroName] [DEBUG])
#   srcs = Variable to define with autogenerated
#          source files
#   hdrs = Variable to define with autogenerated
#          header files
#   PROTOROOT = Root under which the proto files are located. Paths starting
#               from this root are used under OUTPATH as directory structure
#               for the generated files. Defaults to CMAKE_CURRENT_SOURCE_DIR.
#   OUTPATH = Path to store generated files under. Default is
#             CMAKE_CURRENT_BINARY_DIR.
#   EXPORT_MACRO = Tells protoc to generate DLL export definitions using the
#                  specified macro name
#   DEBUG = if set, debug messages will be generated
#
#  ====================================================================


#=============================================================================
# Copyright 2009 Kitware, Inc.
# Copyright 2009 Philip Lowman <philip@yhbt.com>
# Copyright 2008 Esben Mose Hansen, Ange Optimization ApS
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

INCLUDE(ParseArguments)

FUNCTION(PROTOBUF_GENERATE)

    # argument parsing
    PARSE_ARGUMENTS(ARG "PROTOROOT;PROTOFILES;OUTPATH;INCLUDES;EXPORT_MACRO;CPP;JAVA;PYTHON;MATLAB" "DEBUG" ${ARGN})

    IF(NOT ARG_PROTOFILES)
        MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() called without any proto files")
        RETURN()
    ENDIF(NOT ARG_PROTOFILES)
    LIST(LENGTH ARG_PROTOROOT PROTOROOT_LENGTH)
    #IF(PROTOROOT_LENGTH GREATER 1)
    #    MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() called with too many protoroots, only one is allowed")
    #    RETURN()
    #ENDIF()
    LIST(LENGTH ARG_OUTPATH OUTPATH_LENGTH)
    IF(OUTPATH_LENGTH GREATER 1)
        MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() called with too many outpaths, only one is allowed")
        RETURN()
    ENDIF()
    LIST(LENGTH ARG_EXPORT_MACRO EXPORT_MACRO_LENGTH)
    IF(EXPORT_MACRO_LENGTH GREATER 1)
        MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() called with too many export macro names, only one is allowed")
        RETURN()
    ENDIF()
    
    # decide whether to build CPP
    LIST(LENGTH ARG_CPP CPP_LENGTH)
    IF(CPP_LENGTH EQUAL 0)
        SET(BUILD_CPP FALSE)
    ELSE()
        IF(NOT CPP_LENGTH EQUAL 2)
            MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() CPP argument expects two parameters SRC_VAR and HDR_VAR")
            RETURN()
        ENDIF()
        SET(BUILD_CPP TRUE)
        LIST(GET ARG_CPP 0 RESULT_CPP_SRCS)
        LIST(GET ARG_CPP 1 RESULT_CPP_HDRS)
    ENDIF()
    
    # decide whether to build java
    LIST(LENGTH ARG_JAVA JAVA_LENGTH)
    IF(JAVA_LENGTH EQUAL 0)
        SET(BUILD_JAVA FALSE)
    ELSE()
        IF(NOT JAVA_LENGTH EQUAL 1)
            MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() JAVA argument expects one parameter JAVA_VAR")
            RETURN()
        ENDIF()
        SET(BUILD_JAVA TRUE)
        LIST(GET ARG_JAVA 0 RESULT_JAVA)
    ENDIF()
    
    # decide whether to build PYTHON
    LIST(LENGTH ARG_PYTHON PYTHON_LENGTH)
    IF(PYTHON_LENGTH EQUAL 0)
        SET(BUILD_PYTHON FALSE)
    ELSE()
        IF(NOT PYTHON_LENGTH EQUAL 1)
            MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() PYTHON argument expects one parameter PYTHON_VAR")
            RETURN()
        ENDIF()
        SET(BUILD_PYTHON TRUE)
        LIST(GET ARG_PYTHON 0 RESULT_PYTHON)
    ENDIF()
    
    # decide whether to build MATLAB
    LIST(LENGTH ARG_MATLAB MATLAB_LENGTH)
    IF(MATLAB_LENGTH EQUAL 0)
        SET(BUILD_MATLAB FALSE)
    ELSE()
        IF(NOT MATLAB_LENGTH EQUAL 1)
            MESSAGE(SEND_ERROR "Error: PROTOBUF_GENERATE() MATLAB argument expects one parameter MATLAB_VAR")
            RETURN()
        ENDIF()
        SET(BUILD_MATLAB TRUE)
        LIST(GET ARG_MATLAB 0 RESULT_MATLAB)
    ENDIF()
    
    # create proper export macro for CPP if desired
    IF(EXPORT_MACRO_LENGTH EQUAL 1)
        SET(ARG_EXPORT "dllexport_decl=${ARG_EXPORT_MACRO}:")
        MESSAGE(STATUS "Enabling export macro ${ARG_EXPORT_MACRO} for CPP")
    ENDIF()
    
    SET(OUTPATH ${CMAKE_CURRENT_BINARY_DIR})
    IF(OUTPATH_LENGTH EQUAL 1)
        SET(OUTPATH ${ARG_OUTPATH})
    ENDIF()
    SET(PROTOROOTS ${CMAKE_CURRENT_SOURCE_DIR})
    IF(PROTOROOT_LENGTH GREATER 0)
        SET(PROTOROOTS ${ARG_PROTOROOT})
    ENDIF()
    
    SET(ARG_EXPORT "")
    IF(EXPORT_MACRO_LENGTH EQUAL 1)
        SET(ARG_EXPORT "dllexport_decl=${ARG_EXPORT_MACRO}:")
    ENDIF()
    
    # build command line for additional includes paths
    SET(INCLUDE_CMD_LINE)
    FOREACH(P ${ARG_INCLUDES})
        LIST(APPEND INCLUDE_CMD_LINE "--proto_path" ${P})
    ENDFOREACH()
    
    IF(ARG_DEBUG)
        MESSAGE("OUTPATH: ${OUTPATH}")
        MESSAGE("PROTOROOTS: ${PROTOROOTS}")
        MESSAGE("INCLUDE_CMD_LINE: ${INCLUDE_CMD_LINE}")
    ENDIF()

    SET(MATCHED_FILE_PATHS)
    FOREACH(PROTOFILE ${ARG_PROTOFILES})
    
        FILE(TO_CMAKE_PATH ${PROTOFILE} PROTOFILE)
    
        # ensure that the file ends with .proto
        STRING(REGEX MATCH "\\.proto$$" PROTOEND ${PROTOFILE})
        IF(NOT PROTOEND)
            MESSAGE(SEND_ERROR "Proto file '${PROTOFILE}' does not end with .proto")
        ENDIF()
    
        GET_FILENAME_COMPONENT(PROTO_PATH ${PROTOFILE} PATH)
        GET_FILENAME_COMPONENT(ABS_FILE ${PROTOFILE} ABSOLUTE)
        GET_FILENAME_COMPONENT(FILE_WE ${PROTOFILE} NAME_WE)
        
        STRING(LENGTH ${ABS_FILE} ABS_FILE_LENGTH)
        
        IF(ARG_DEBUG)
            MESSAGE("file ${PROTOFILE}:")
            MESSAGE("  PATH=${PROTO_PATH}")
            MESSAGE("  ABS_FILE=${ABS_FILE}")
            MESSAGE("  FILE_WE=${FILE_WE}")
            MESSAGE("  PROTOROOTS=${PROTOROOTS}")
        ENDIF()
        
        # find out if the file is in one of the specified proto root
        # we mimic the protoc logic here by taking the first matching proto_path
        SET(MATCH_PATH)
        FOREACH(ROOT ${PROTOROOTS})
        
            IF(ARG_DEBUG)
                MESSAGE("  ROOT=${ROOT}")
            ENDIF()
        
            FILE(RELATIVE_PATH REL_ABS ${ROOT} ${ABS_FILE})
            STRING(LENGTH ${REL_ABS} REL_LENGTH)
            
            IF(ARG_DEBUG)
                MESSAGE("    REL_ABS=${REL_ABS}")
                MESSAGE("    REL_LENGTH=${REL_LENGTH}")
            ENDIF()
            
            IF(${REL_LENGTH} GREATER 0 AND ${REL_LENGTH} LESS ${ABS_FILE_LENGTH})
                # we did not need to go directories up, hence the path is shorter
                # and this is a match... bad assumption but works
                SET(MATCH_PATH ${REL_ABS})
                SET(MATCH_ROOT ${ROOT})
                IF(ARG_DEBUG)
                    MESSAGE("  MATCH_ROOT=${MATCH_ROOT}")
                ENDIF()
                BREAK()
            ENDIF()
            
        ENDFOREACH()
        
        IF(ARG_DEBUG)
            MESSAGE("  MATCH_PATH=${MATCH_PATH}")
        ENDIF()
        
        IF(NOT MATCH_PATH)
            MESSAGE(SEND_ERROR "Proto file '${PROTOFILE}' is not in protoroots '${PROTOROOTS}'")
        ENDIF()
        LIST(APPEND MATCHED_FILE_PATHS ${ABS_FILE})
        
        # build the result file name
        FILE(RELATIVE_PATH ROOT_CLEANED_FILE ${MATCH_ROOT} ${ABS_FILE})
        IF(ARG_DEBUG)
            MESSAGE("  ROOT_CLEANED_FILE=${ROOT_CLEANED_FILE}")
        ENDIF()
        STRING(REGEX REPLACE "\\.proto$$" "" EXT_CLEANED_FILE ${ROOT_CLEANED_FILE})
        IF(ARG_DEBUG)
            MESSAGE("  EXT_CLEANED_FILE=${EXT_CLEANED_FILE}")
        ENDIF()
        
        SET(CPP_FILE "${OUTPATH}/${EXT_CLEANED_FILE}.pb.cc")
        SET(HDR_FILE "${OUTPATH}/${EXT_CLEANED_FILE}.pb.h")
        SET(PYTHON_FILE "${OUTPATH}/${EXT_CLEANED_FILE}_pb2.py")
        
        # determine the java file name
        FILE(READ ${PROTOFILE} PROTO_CONTENT)
        
        # first the package
        # TODO jwienke: ignore comments... see below TODO
        SET(PACKAGE_REGEX "package[\t ]+([^;\n\r]+);")
        STRING(REGEX MATCH ${PACKAGE_REGEX} PACKAGE_LINE ${PROTO_CONTENT})
        IF(ARG_DEBUG)
            MESSAGE("  PACKAGE_LINE=${PACKAGE_LINE}")
        ENDIF()
        SET(PACKAGE "")
        IF(PACKAGE_LINE)
            STRING(REGEX REPLACE ${PACKAGE_REGEX} "\\1" PACKAGE ${PACKAGE_LINE})
        ENDIF()
        STRING(REPLACE "." "/" JAVA_PACKAGE_PATH "${PACKAGE}")
        IF(ARG_DEBUG)
            MESSAGE("  PACKAGE=${PACKAGE}")
            MESSAGE("  JAVA_PACKAGE_PATH=${JAVA_PACKAGE_PATH}")
        ENDIF()
        
        # then the java class name
        
        # this is the default
        # TODO jwienke: how to integrate that this line must not start with //?
        #               cmake regex are strange, because ^ and $ match beginning
        #               and end of file and not of each line
        SET(JAVA_CLASS_REGEX "option[\t ]+java_outer_classname[\t ]+=[\t ]+\"([^\"]+)\"")
        STRING(REGEX MATCH ${JAVA_CLASS_REGEX} JAVA_CLASS_LINE ${PROTO_CONTENT})
        IF(ARG_DEBUG)
            MESSAGE("  JAVA_CLASS_LINE=${JAVA_CLASS_LINE}")
        ENDIF()
        SET(JAVA_CLASS ${FILE_WE})
        IF(JAVA_CLASS_LINE)
            STRING(REGEX REPLACE ${JAVA_CLASS_REGEX} "\\1" JAVA_CLASS ${JAVA_CLASS_LINE})
            # Now that we have the real java class name, this must be replaced
            # in the original file name proposal. However, the subpath in the
            # file system is also part of EXT_CLEANED_FILE. Hence, we need to
            # do some replace logic again...
        ENDIF()
        IF(ARG_DEBUG)
            MESSAGE("  JAVA_CLASS=${JAVA_CLASS}")
        ENDIF()
        
        # finally deduce the real java name
        SET(JAVA_FILE "${OUTPATH}/${JAVA_PACKAGE_PATH}/${JAVA_CLASS}.java")

        IF(ARG_DEBUG)
            MESSAGE("  CPP_FILE=${CPP_FILE}")
            MESSAGE("  HDR_FILE=${HDR_FILE}")
            MESSAGE("  JAVA_FILE=${JAVA_FILE}")
            MESSAGE("  PYTHON_FILE=${PYTHON_FILE}")
        ENDIF()
        
        # generate and use a list of protoroot arguments to pass to protoc
        SET(ROOT_ARGS)
        FOREACH(ROOT ${PROTOROOTS})
            LIST(APPEND ROOT_ARGS "--proto_path" ${ROOT})
        ENDFOREACH()
        
        IF(BUILD_CPP)
            LIST(APPEND CPP_SRCS "${CPP_FILE}")
            LIST(APPEND CPP_HDRS "${HDR_FILE}")
    
            ADD_CUSTOM_COMMAND(
                OUTPUT "${CPP_FILE}"
                       "${HDR_FILE}"
                COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPATH}
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
                ARGS "--cpp_out=${ARG_EXPORT}${OUTPATH}" ${ROOT_ARGS} ${INCLUDE_CMD_LINE} "${ABS_FILE}"
                DEPENDS ${ABS_FILE}
                COMMENT "Running C++ protocol buffer compiler on ${ABS_FILE} with root ${MATCH_ROOT}, generating: ${CPP_FILE}"
                VERBATIM)
        ENDIF()
        
        IF(BUILD_JAVA)
            LIST(APPEND JAVA_FILES "${JAVA_FILE}")
    
            ADD_CUSTOM_COMMAND(
                OUTPUT "${JAVA_FILE}"
                COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPATH}
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
                ARGS "--java_out=${OUTPATH}" ${ROOT_ARGS} ${INCLUDE_CMD_LINE} "${ABS_FILE}"
                DEPENDS ${ABS_FILE}
                COMMENT "Running Java protocol buffer compiler on ${ABS_FILE} with root ${MATCH_ROOT}, generating: ${JAVA_FILE}"
                VERBATIM)
        ENDIF()
        
        IF(BUILD_PYTHON)
            LIST(APPEND PYTHON_FILES "${PYTHON_FILE}")
    
            ADD_CUSTOM_COMMAND(
                OUTPUT "${PYTHON_FILE}"
                COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPATH}
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
                ARGS "--python_out=${OUTPATH}" ${ROOT_ARGS} ${INCLUDE_CMD_LINE} "${ABS_FILE}"
                DEPENDS ${ABS_FILE}
                COMMENT "Running Python protocol buffer compiler on ${ABS_FILE} with root ${MATCH_ROOT}, generating: ${PYTHON_FILE}"
                VERBATIM)
        ENDIF()
            
    ENDFOREACH()

    IF(BUILD_CPP)
        SET_SOURCE_FILES_PROPERTIES(${${CPP_SRCS}} ${${CPP_HDRS}} PROPERTIES GENERATED TRUE)
        SET(${RESULT_CPP_SRCS} ${CPP_SRCS} PARENT_SCOPE)
        SET(${RESULT_CPP_HDRS} ${CPP_HDRS} PARENT_SCOPE)
    ENDIF()

    IF(BUILD_JAVA)
        SET_SOURCE_FILES_PROPERTIES(${${JAVA_FILES}} PROPERTIES GENERATED TRUE)
        SET(${RESULT_JAVA} ${JAVA_FILES} PARENT_SCOPE)
    ENDIF()

    IF(BUILD_PYTHON)
        SET_SOURCE_FILES_PROPERTIES(${${PYTHON_FILES}} PROPERTIES GENERATED TRUE)
        SET(${RESULT_PYTHON} ${PYTHON_FILES} PARENT_SCOPE)
    ENDIF()
    
    IF(BUILD_MATLAB)
        ADD_CUSTOM_COMMAND(
            OUTPUT ${OUTPATH}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPATH}
            COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
            ARGS "--matlab_out=${OUTPATH}" ${ROOT_ARGS} ${INCLUDE_CMD_LINE} ${MATCHED_FILE_PATHS}
            DEPENDS ${MATCHED_FILE_PATHS}
            COMMENT "Running Matlab protocol buffer compiler to generate files in ${OUTPATH}"
            VERBATIM)
    ENDIF()

    IF(BUILD_MATLAB)
        SET(${RESULT_MATLAB} ${OUTPATH} PARENT_SCOPE)
    ENDIF()
    
ENDFUNCTION()

MACRO(PROTOBUF_GENERATE_CPP SRCS HDRS)
    PROTOBUF_GENERATE(${ARGN} CPP ${SRCS} ${HDRS})
ENDMACRO()

FIND_PATH(PROTOBUF_INCLUDE_DIR NAMES google/protobuf/service.h
          HINTS "${PROTOBUF_ROOT}/include"
          DOC "The Google Protocol Buffers Headers")

# Google's provided vcproj files generate libraries with a "lib"
# prefix on Windows
IF(WIN32)
    SET(PROTOBUF_ORIG_FIND_LIBRARY_PREFIXES "${CMAKE_FIND_LIBRARY_PREFIXES}")
    SET(CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
ENDIF()

FIND_LIBRARY(PROTOBUF_LIBRARY NAMES protobuf
             HINTS "${PROTOBUF_ROOT}/bin"
                   "${PROTOBUF_ROOT}/lib"
             DOC "The Google Protocol Buffers Library"
)
FIND_LIBRARY(PROTOBUF_PROTOC_LIBRARY NAMES protoc
             HINTS "${PROTOBUF_ROOT}/bin"
                   "${PROTOBUF_ROOT}/lib"
             DOC "The Google Protocol Buffers Compiler Library"
)
FIND_PROGRAM(PROTOBUF_PROTOC_EXECUTABLE NAMES protoc
             HINTS "${PROTOBUF_ROOT}/bin"
             DOC "The Google Protocol Buffers Compiler"
)
IF(PROTOBUF_PROTOC_EXECUTABLE AND NOT PROTOBUF_PROTOC_MATLAB)
    # check whether this protoc version supports matlab
    EXECUTE_PROCESS(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} "-h"
                    ERROR_VARIABLE PROTOC_HELP_TEXT
                    OUTPUT_QUIET)
    STRING(REGEX MATCH "--matlab_out" PROTOC_MATLAB_OUT "${PROTOC_HELP_TEXT}")
    IF(PROTOC_MATLAB_OUT)
        SET(PROTOBUF_PROTOC_MATLAB TRUE CACHE BOOL "Whether protoc is able to generate matlab output.")
        MESSAGE(STATUS "protoc supports matlab")
    ELSE()
        SET(PROTOBUF_PROTOC_MATLAB FALSE CACHE BOOL "Whether protoc is able to generate matlab output.")
        MESSAGE(STATUS "protoc does not support matlab")
    ENDIF()
ENDIF()
FIND_FILE(PROTOBUF_JAVA_LIBRARY
          NAMES ${PROTOBUF_JAVA_NAME} protobuf.jar protobuf-java.jar
          HINTS ${PROTOBUF_JAVA_ROOT}
                "${PROTOBUF_JAVA_ROOT}/share/java"
                "/usr/share/java"
                "/usr/share/java/protobuf-java"
                "/usr/share/protobuf/lib"
                "${CMAKE_INSTALL_PREFIX}/lib/java"
                "${CMAKE_INSTALL_PREFIX}/share/java")

MARK_AS_ADVANCED(PROTOBUF_INCLUDE_DIR
                 PROTOBUF_LIBRARY
                 PROTOBUF_PROTOC_LIBRARY
                 PROTOBUF_PROTOC_EXECUTABLE
                 PROTOBUF_JAVA_LIBRARY
                 PROTOBUF_PROTOC_MATLAB)

# Restore original find library prefixes
IF(WIN32)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "${PROTOBUF_ORIG_FIND_LIBRARY_PREFIXES}")
ENDIF()

INCLUDE(FindPackageHandleStandardArgs) 
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PROTOBUF DEFAULT_MSG PROTOBUF_LIBRARY PROTOBUF_INCLUDE_DIR)

IF(PROTOBUF_FOUND)
    SET(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIR})
    SET(PROTOBUF_LIBRARIES    ${PROTOBUF_LIBRARY})
ENDIF()
