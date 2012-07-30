# -*- mode: cmake -*-

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

PROJECT("RSC")

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(InstallFilesRecursive)
INCLUDE(EnableCoverageReport)
INCLUDE(EnableSlocCount)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)

# --- user options ---

OPTION(BUILD_TESTS "Decide if the test suite shall be built or not" ON)
OPTION(BUILD_EXAMPLES "Decide if the examples shall be built or not" ON)

# --- global definitions ---

# version information
SET(RSC_VERSION_MAJOR "0")
SET(RSC_VERSION_MINOR "5")
SET(RSC_VERSION_PATCH "0")
SET(RSC_VERSION "${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}.${RSC_VERSION_PATCH}")
SET(RSC_ABI_VERSION "4")

FIND_PACKAGE(Subversion)
INCLUDE(SubversionIsWc)
SET(RSC_WC_REVISION "archive")
Subversion_IS_WC(${CMAKE_CURRENT_SOURCE_DIR} IS_SVN_VERSION)
MESSAGE(STATUS "This is a checked out version: ${IS_SVN_VERSION}")
IF(Subversion_FOUND AND IS_SVN_VERSION)
    Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} RSC)
    MESSAGE(STATUS "Current revision is ${RSC_WC_REVISION}")
ENDIF()

SET(RSC_NAME "rsc")
SET(RSC_TEST_NAME "rsctest")

SET(RSC_CMAKE_MODULE_PATH "share/cmake/Modules")

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
    LIST(APPEND EXTERNAL_INCLUDES "\${includedir}/rsc/3rdparty/boost.uuid/")
ENDIF()
STRING(REPLACE ";" " -I" EXTERNAL_INCLUDE_COMMANDS "${EXTERNAL_INCLUDES}")
STRING(LENGTH "${EXTERNAL_INCLUDE_COMMANDS}" EXT_LENGTH)
IF(${EXT_LENGTH} GREATER 0)
    SET(EXTERNAL_INCLUDE_COMMANDS "-I${EXTERNAL_INCLUDE_COMMANDS}")
ENDIF()
CONFIGURE_FILE(rsc.pc.in ${CMAKE_BINARY_DIR}/rsc.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/rsc.pc DESTINATION lib/pkgconfig)

# --- cmake config files ---

SET(LIB_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
IF(CMAKE_LINK_LIBRARY_SUFFIX)
    SET(LIB_SUFFIX ${CMAKE_LINK_LIBRARY_SUFFIX})
ENDIF()
CONFIGURE_FILE(RSCConfig.cmake.in ${CMAKE_BINARY_DIR}/RSCConfig.cmake @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/RSCConfig.cmake DESTINATION share/rsc)
CONFIGURE_FILE(RSCConfigVersion.cmake.in ${CMAKE_BINARY_DIR}/RSCConfigVersion.cmake @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/RSCConfigVersion.cmake DESTINATION share/rsc)

EXPORT(TARGETS ${RSC_NAME} FILE "${CMAKE_BINARY_DIR}/RSCDepends.cmake")
INSTALL(EXPORT RSCDepends
        DESTINATION "share/rsc")

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
  ENABLE_IDS style)


# --- package ---
INCLUDE(CheckLSBTypes)

SET(CPACK_GENERATOR "STGZ;TGZ;TZ")
SET(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${RSC_VERSION}_${LSB_CODENAME}_${LSB_PROCESSOR_ARCH})
SET(CPACK_PACKAGE_VENDOR "CoR-Lab Bielefeld University")
SET(CPACK_PACKAGE_VERSION_MAJOR ${RSC_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${RSC_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${RSC_VERSION_PATCH})
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    SET(CPACK_GENERATOR "${CPACK_GENERATOR};DEB")
    SET(CPACK_DEBIAN_PACKAGE_NAME "rsc")
    SET(CPACK_DEBIAN_PACKAGE_VERSION ${RSC_VERSION})
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sebastian Wrede (swrede@techfak.uni-bielefeld.de)")
    SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "RSC is a project focused on all aspects of reusable library functions in robotics systems. RSC is meant to improve the C++ std lib for robotic systems without pulling in many additional dependencies.")
    SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-signals-dev, cmake-data, cmake, libc6, libboost-program-options-dev, libboost-filesystem-dev, libboost-thread-dev")
    SET(CPACK_DEBIAN_PACKAGE_SUGGESTS "doxygen, cppcheck, sloccount, lcov")
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

INCLUDE(CPack)
