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

# --- user options ---

OPTION(BUILD_TESTS "Decide if the test suite shall be built or not" ON)
OPTION(BUILD_EXAMPLES "Decide if the examples shall be built or not" ON)
OPTION(INSTALL_TOOLCHAINS "Decide if cmake toolchain files should be installed" ON)
OPTION(EXPORT_TO_CMAKE_PACKAGE_REGISTRY "If set to ON, RSC will be exported to the CMake user package registry so that downstream projects automatically find the workspace location in find_package calls." OFF)

# --- global definitions ---

DEFINE_PROJECT_VERSION(RSC_ 0 8 0 "archive")

MESSAGE(STATUS "This is RSC version: ${RSC_VERSION}-${RSC_WC_REVISION}")

SET(RSC_ABI_VERSION "4")
SET(SO_VERSION "${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}")

SET(RSC_NAME "rsc")
SET(RSC_TEST_NAME "rsctest")

SET(INSTALL_PATH_PREFIX "rsc${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}"
    CACHE STRING "Prefix path applied to all non-versioned installed files in order to prevent version clashes.")

SET(RSC_CMAKE_PATH "share/${INSTALL_PATH_PREFIX}/cmake")
SET(RSC_CMAKE_MODULE_PATH "${RSC_CMAKE_PATH}/Modules")
SET(RSC_CMAKE_TOOLCHAIN_PATH "${RSC_CMAKE_PATH}/Toolchains")

SET(OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)
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

FIND_PACKAGE(Boost ${Boost_USE_VERSION} REQUIRED thread filesystem signals program_options system)

SET(USE_INTERNAL_BOOST_UUID FALSE CACHE BOOL "Wether to use the internal version of boost.uuid or not")
IF(NOT USE_INTERNAL_BOOST_UUID)
	MESSAGE(STATUS "Trying to find an external version of Boost.uuid")
	FIND_PACKAGE(BoostUUID)
	MESSAGE(STATUS "Boost.uuid found in system: ${BOOSTUUID_FOUND}")
ENDIF()
IF(NOT BOOSTUUID_FOUND OR USE_INTERNAL_BOOST_UUID)
    MESSAGE(STATUS "Using an internal version of Boost.uuid. Remember that your client libraries have to use this version, too.")
    SET(USE_INTERNAL_BOOST_UUID TRUE CACHE BOOL "Wether to use the internal version of boost.uuid or not" FORCE)
    SET(BOOSTUUID_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/boost.uuid" CACHE PATH "Internal include path for Boost.uuid" FORCE)
ENDIF()
INCLUDE_DIRECTORIES(BEFORE SYSTEM ${Boost_INCLUDE_DIRS} ${BOOSTUUID_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# --- source code ---

INCLUDE_DIRECTORIES(BEFORE src ${CMAKE_CURRENT_BINARY_DIR}/src)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(3rdparty)
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

# --- pkgconfig file ---

SET(EXTERNAL_INCLUDES ${Boost_INCLUDE_DIRS})
IF(NOT USE_INTERNAL_BOOST_UUID)
    LIST(APPEND EXTERNAL_INCLUDES ${BOOSTUUID_INCLUDE_DIRS})
ELSE()
    LIST(APPEND EXTERNAL_INCLUDES "\${includedir}/${INSTALL_PATH_PREFIX}/3rdparty/boost.uuid/")
ENDIF()
STRING(REPLACE ";" " -I" EXTERNAL_INCLUDE_COMMANDS "${EXTERNAL_INCLUDES}")
STRING(LENGTH "${EXTERNAL_INCLUDE_COMMANDS}" EXT_LENGTH)
IF(${EXT_LENGTH} GREATER 0)
    SET(EXTERNAL_INCLUDE_COMMANDS "-I${EXTERNAL_INCLUDE_COMMANDS}")
ENDIF()
CONFIGURE_FILE(rsc.pc.in "${CMAKE_BINARY_DIR}/rsc${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}.pc" @ONLY)
INSTALL(FILES "${CMAKE_BINARY_DIR}/rsc${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}.pc" DESTINATION lib/pkgconfig)

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
CONFIGURE_FILE(RSCConfig.cmake.in "${CMAKE_BINARY_DIR}/RSCConfig.cmake" @ONLY)
INSTALL(FILES "${CMAKE_BINARY_DIR}/RSCConfig.cmake" DESTINATION "share/${INSTALL_PATH_PREFIX}")
CONFIGURE_FILE(RSCConfigVersion.cmake.in "${CMAKE_BINARY_DIR}/RSCConfigVersion.cmake" @ONLY)
INSTALL(FILES "${CMAKE_BINARY_DIR}/RSCConfigVersion.cmake" DESTINATION "share/${INSTALL_PATH_PREFIX}")

EXPORT(TARGETS ${RSC_NAME} FILE "${CMAKE_BINARY_DIR}/RSCDepends.cmake")
INSTALL(EXPORT RSCDepends
        DESTINATION "share/${INSTALL_PATH_PREFIX}")

INSTALL(FILES COPYING.txt DESTINATION "share/doc/${INSTALL_PATH_PREFIX}" RENAME copyright)

# --- enable unit tests if desired ---

IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

# --- documentation generation ---

GENERATE_DOXYGEN(VERSION "${RSC_VERSION}")

# --- coverage ---

ENABLE_COVERAGE_REPORT(TARGETS ${RSC_NAME} TESTS ${RSC_TEST_NAME} FILTER "*3rdparty*" "*test/*")

# --- sloccount ---

ENABLE_SLOCCOUNT(FOLDERS src test examples)

# --- cppcheck ---

GENERATE_CPPCHECK(SOURCES src test examples
                  "${CMAKE_CURRENT_BINARY_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/test" "${CMAKE_CURRENT_BINARY_DIR}/examples"
                  ENABLE_IDS style
                  INLINE_SUPPRESSION)


# --- package ---

# default configuration, platform independent
SET(CPACK_PACKAGE_VERSION_MAJOR ${RSC_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${RSC_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${RSC_VERSION_PATCH})

SET(CPACK_PACKAGE_VENDOR "CoR-Lab Bielefeld University")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")

INCLUDE(ProvideFlexibleCPack)
