IF(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    MESSAGE(FATAL_ERROR "Cannot configure CPack to generate Debian packages on non-linux systems.")
ENDIF()

# To create a proper Debian/Ubuntu package, the following CMake
# options should be used:

# SET(BUILD_TESTS OFF)
# SET(BUILD_EXAMPLES OFF)

# SET(CMAKE_BUILD_TYPE Release)
SET(CPACK_STRIP_FILES TRUE)

# Actual packaging options

INCLUDE(CheckLSBTypes)

SET(VERSION_SUFFIX "${RSC_VERSION_MAJOR}.${RSC_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION   "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}${CPACK_PACKAGE_REVISION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}${VERSION_SUFFIX}-${CPACK_PACKAGE_VERSION}_${LSB_CODENAME}_${LSB_PROCESSOR_ARCH}")

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "rsc${VERSION_SUFFIX}")
SET(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sebastian Wrede <swrede@techfak.uni-bielefeld.de>")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Library of reusable C++ and CMake code.
 RSC is a project focused on all aspects of reusable library functions
 in robotics systems. RSC is meant to improve the C++ std lib for
 robotic systems without pulling in many additional dependencies.")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
SET(CPACK_DEBIAN_PACKAGE_CONFLICTS "rsc0.6, rsc0.5, rsc0.4")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-signals-dev, libc6, libboost-program-options-dev, libboost-filesystem-dev, libboost-thread-dev")
SET(CPACK_DEBIAN_PACKAGE_SUGGESTS "cmake-data, cmake, doxygen, cppcheck, sloccount, lcov")
