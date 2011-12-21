
IF(BUILD_TESTS)

    # reuse the external project module to flexibly download source code but we
    # will not use it for building the code
    INCLUDE(ExternalProject)
    ExternalProject_Add(gmock-src
            PREFIX "${CMAKE_CURRENT_BINARY_DIR}/gmock"
            URL ${GMOCK_SOURCE_LOCATION}
            BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/gmock/build"
            CONFIGURE_COMMAND ""
            #BUILD_COMMAND ""
            INSTALL_COMMAND "")
    # However, downloading is done at build time, hence letting a library target
    # simply depend on the relevant source files in gmock does not work, as they
    # cannot be found in the source tree and there is no target which builds.
    # For these reasons we add an artifical target which generates them and
    # depends on the external project.
    SET(GMOCK_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/gmock/src/gmock-src/gtest/src/gtest-all.cc"
                      "${CMAKE_CURRENT_BINARY_DIR}/gmock/src/gmock-src/src/gmock-all.cc")
    ADD_CUSTOM_COMMAND(OUTPUT ${GMOCK_SOURCES}
                       COMMAND ${CMAKE_COMMAND} -h
                       DEPENDS gmock-src)

    INCLUDE_DIRECTORIES(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/gmock/src/gmock-src/gtest
                               ${GMOCK_INCLUDE_DIR}
                               "${CMAKE_CURRENT_BINARY_DIR}/gmock/src/gmock-src
                               ${GTEST_INCLUDE_DIR})

    ADD_LIBRARY(${GMOCK_LIBRARY_NAME} STATIC ${GMOCK_SOURCES})

ENDIF()

IF(USE_INTERNAL_BOOST_UUID)
    FILE(GLOB_RECURSE UUID_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.hpp)
    INSTALL(FILES ${UUID_FILES} DESTINATION include/rsc/3rdparty/boost.uuid/boost/uuid)
ENDIF()
