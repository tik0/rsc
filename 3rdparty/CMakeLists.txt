
IF(BUILD_TESTS)

    INCLUDE_DIRECTORIES(BEFORE ${GMOCK_DIR}
                               ${GMOCK_INCLUDE_DIR}
                               ${GTEST_DIR}
                               ${GTEST_INCLUDE_DIR})

    ADD_LIBRARY(${GMOCK_LIBRARY_NAME} STATIC gmock/gtest/src/gtest-all.cc
                                             gmock/src/gmock-all.cc)

ENDIF()

IF(USE_INTERNAL_BOOST_UUID)
    FILE(GLOB_RECURSE UUID_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.hpp)
    INSTALL(FILES ${UUID_FILES} DESTINATION include/rsc/3rdparty/boost.uuid/boost/uuid)
ENDIF()
