# -*- mode: cmake -*-

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

PROJECT("RSC")

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(InstallFilesRecursive)
INCLUDE(EnableCoverageReport)
INCLUDE(EnableSlocCount)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)
INCLUDE(DefineProjectVersion)
INCLUDE(CurrentDate)

# --- user options ---

OPTION(BUILD_TESTS "Decide if the test suite shall be built or not" ON)
OPTION(BUILD_EXAMPLES "Decide if the examples shall be built or not" ON)
OPTION(INSTALL_TOOLCHAINS "Decide if CMake toolchain files should be installed" ON)
OPTION(EXPORT_TO_CMAKE_PACKAGE_REGISTRY "If set to ON, RSC will be exported to the CMake user package registry so that downstream projects automatically find the workspace location in find_package calls." OFF)
OPTION(ENCODE_VERSION "If set to ON, install pathes and library name will have the version encoded ('rsc{major,minor}' instead of 'rsc') to allow parallel installation of different versions." ON)

# --- global definitions ---

SET(VERSION_MAJOR "0" CACHE STRING "Major project version part")
SET(VERSION_MINOR "14" CACHE STRING "Minor project version part")

DEFINE_PROJECT_VERSION(RSC_ ${VERSION_MAJOR} ${VERSION_MINOR} 0 "archive")
CURRENT_DATE(RSC_BUILD_DATE)

MESSAGE(STATUS "This is RSC version: ${RSC_VERSION}-${RSC_WC_REVISION}")

SET(RSC_ABI_VERSION "4")
SET(SO_VERSION "${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}")

IF(ENCODE_VERSION)
    SET(RSC_NAME "rsc${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}")
ELSE(ENCODE_VERSION)
    SET(RSC_NAME "rsc")
ENDIF(ENCODE_VERSION)
SET(VERSION_SUFFIX "${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}")
SET(INSTALL_PATH_PREFIX "${RSC_NAME}"
    CACHE STRING "Prefix path applied to all non-versioned installed files in order to prevent version clashes.")
SET(INSTALL_PATH_PREFIX_CMAKE "rsc-cmake${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}"
    CACHE STRING "Prefix path applied to the config files for the CMake library.")

SET(RSC_TEST_NAME "rsctest")

SET(RSC_CMAKE_PATH "share/${INSTALL_PATH_PREFIX}/cmake")
SET(RSC_CMAKE_MODULE_PATH "${RSC_CMAKE_PATH}/Modules")
SET(RSC_CMAKE_TOOLCHAIN_PATH "${RSC_CMAKE_PATH}/Toolchains")

SET(OUTPUT_PATH "${CMAKE_BINARY_DIR}/build")
SET(ARCHIVE_OUTPUT_PATH ${OUTPUT_PATH})
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_PATH})
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

# --- global compiler flags ---

INCLUDE(PedanticCompilerWarnings)

IF(WIN32)
    ADD_DEFINITIONS(/D_USE_MATH_DEFINES)
ENDIF()

CHECK_INCLUDE_FILE("sys/types.h" HAVE_TYPES_H)
IF(HAVE_TYPES_H)
    ADD_DEFINITIONS(-DHAVE_TYPES_H)
ENDIF()
CHECK_INCLUDE_FILE("sys/wait.h" HAVE_WAIT_H)
IF(HAVE_WAIT_H)
    ADD_DEFINITIONS(-DHAVE_WAIT_H)
ENDIF()
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
IF(HAVE_UNISTD_H)
    ADD_DEFINITIONS(-DHAVE_UNISTD_H)
ENDIF()

# decide how to do name demangling
CHECK_INCLUDE_FILE_CXX("cxxabi.h" HAVE_CXXABI_H)
IF(HAVE_CXXABI_H)
    ADD_DEFINITIONS(-DDEMANGLE_GCC)
ELSEIF(MSVC)
    ADD_DEFINITIONS(-DDEMANGLE_MSVC)
ELSE()
    MESSAGE(SEND_ERROR "No demangling solution found for the system.")
ENDIF()

# --- dependency handling ---

FIND_PACKAGE(Threads REQUIRED)

SET(Boost_USE_VERSION 1.38 CACHE INTERNAL "Boost Version to use")
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS OFF)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)

IF(BOOST_ROOT)
    SET(Boost_NO_SYSTEM_PATHS ON)
ENDIF()

# determine the required libraries for boost based on its version
FIND_PACKAGE(Boost ${Boost_USE_VERSION} REQUIRED)
SET(BOOST_COMPONENTS date_time thread filesystem signals program_options system regex)
IF(Boost_VERSION STREQUAL 105000 OR Boost_VERSION STRGREATER 105000)
    LIST(APPEND BOOST_COMPONENTS chrono)
ENDIF()
FIND_PACKAGE(Boost ${Boost_USE_VERSION} REQUIRED ${BOOST_COMPONENTS})
INCLUDE_DIRECTORIES(BEFORE SYSTEM ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# --- source code ---

INCLUDE_DIRECTORIES(BEFORE src "${CMAKE_CURRENT_BINARY_DIR}/src")

ADD_SUBDIRECTORY(src)
IF(BUILD_TESTS)
    INCLUDE(ProvideGoogleMock)
    IF(GMOCK_AVAILABLE)
        ADD_SUBDIRECTORY(test)
    ELSE()
        MESSAGE(WARNING "Could not build unit tests even though desired because Google Mock could not be installed.")
    ENDIF()
ENDIF()
IF(BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(examples)
ENDIF()

ADD_SUBDIRECTORY(cmake)


# --- package ---
# default configuration, independent of packaging mechanism
SET(CPACK_PACKAGE_VERSION_MAJOR ${RSC_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${RSC_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${RSC_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION       "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_PACKAGE_VENDOR        "CoR-Lab Bielefeld University")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")
INCLUDE(ProvideFlexibleCPack)

# --- pkgconfig file ---

IF(UNIX)
    SET(EXTERNAL_INCLUDES ${Boost_INCLUDE_DIRS})
    STRING(REPLACE ";" " -I" EXTERNAL_INCLUDE_COMMANDS "${EXTERNAL_INCLUDES}")
    STRING(LENGTH "${EXTERNAL_INCLUDE_COMMANDS}" EXT_LENGTH)
    IF(${EXT_LENGTH} GREATER 0)
        SET(EXTERNAL_INCLUDE_COMMANDS "-I${EXTERNAL_INCLUDE_COMMANDS}")
    ENDIF()
    CONFIGURE_FILE(rsc.pc.in "${CMAKE_BINARY_DIR}/${RSC_NAME}.pc" @ONLY)
    INSTALL(FILES "${CMAKE_BINARY_DIR}/${RSC_NAME}.pc" DESTINATION lib/pkgconfig)
ENDIF()

# --- cmake config files ---

IF(EXPORT_TO_CMAKE_PACKAGE_REGISTRY)
    MESSAGE(STATUS "Exported RSC to CMake package registry")
    EXPORT(PACKAGE RSC)
ENDIF()

CONFIGURE_FILE(RSCBuildTreeSettings.cmake.in
               "${CMAKE_BINARY_DIR}/RSCBuildTreeSettings.cmake" @ONLY)

SET(LIB_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
IF(CMAKE_LINK_LIBRARY_SUFFIX)
    SET(LIB_SUFFIX ${CMAKE_LINK_LIBRARY_SUFFIX})
ENDIF()
CONFIGURE_FILE(RSCConfig.cmake.in
               "${CMAKE_BINARY_DIR}/RSCConfig.cmake"
               @ONLY)
CONFIGURE_FILE(RSC-CMakeConfig.cmake.in
               "${CMAKE_BINARY_DIR}/RSC-CMakeConfig.cmake"
               @ONLY)
INSTALL(FILES "${CMAKE_BINARY_DIR}/RSC-CMakeConfig.cmake"
              "${CMAKE_BINARY_DIR}/RSC-CMakeConfigVersion.cmake"
        DESTINATION "share/${INSTALL_PATH_PREFIX_CMAKE}")
CONFIGURE_FILE(RSCConfigVersion.cmake.in
               "${CMAKE_BINARY_DIR}/RSCConfigVersion.cmake"
               @ONLY)
CONFIGURE_FILE(RSCConfigVersion.cmake.in
               "${CMAKE_BINARY_DIR}/RSC-CMakeConfigVersion.cmake"
               @ONLY)
INSTALL(FILES "${CMAKE_BINARY_DIR}/RSCConfig.cmake"
              "${CMAKE_BINARY_DIR}/RSCConfigVersion.cmake"
        DESTINATION "share/${INSTALL_PATH_PREFIX}")

EXPORT(TARGETS ${RSC_NAME} FILE "${CMAKE_BINARY_DIR}/RSCDepends.cmake")
INSTALL(EXPORT RSCDepends
        DESTINATION "share/${INSTALL_PATH_PREFIX}")

# --- enable unit tests if desired ---

IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

# --- documentation generation ---

GENERATE_DOXYGEN(VERSION "${RSC_VERSION}")

# --- coverage ---

ENABLE_COVERAGE_REPORT(TARGETS ${RSC_NAME}
                       TESTS  ${RSC_TEST_NAME}
                       FILTER "*test/*")

# --- sloccount ---

ENABLE_SLOCCOUNT(FOLDERS src test examples)

# --- cppcheck ---

GENERATE_CPPCHECK(SOURCES src test examples
                  "${CMAKE_CURRENT_BINARY_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/test" "${CMAKE_CURRENT_BINARY_DIR}/examples"
                  ENABLE_IDS style
                  INLINE_SUPPRESSION)
