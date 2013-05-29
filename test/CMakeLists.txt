ENABLE_TESTING()

SET(TEST_RESULT_DIR ${CMAKE_BINARY_DIR}/testresults)
SET(PKGCONFIG_TEST_NAME "${RSC_TEST_NAME}_pkgconfig")
SET(TEST_PLUGIN_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/testplugins")
SET(PLUGIN_CALL_FILE "${CMAKE_CURRENT_BINARY_DIR}/testplugins/callfile")

INCLUDE_DIRECTORIES(BEFORE SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/src)

SET(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
CONFIGURE_FILE(testconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.h)

# --- library test ---

SET(REGISTRY_TEST_LIB_NAME "registry")
ADD_LIBRARY(${REGISTRY_TEST_LIB_NAME} SHARED "rsc/misc/testRegistry.cpp" "rsc/misc/testRegistry.h")
TARGET_LINK_LIBRARIES(${REGISTRY_TEST_LIB_NAME} ${RSC_NAME})

SET(REGISTREE_TEST_LIB_NAME "registree")
ADD_LIBRARY(${REGISTREE_TEST_LIB_NAME} SHARED "rsc/misc/RegistreeLib.cpp" "rsc/misc/RegistreeLib.h")
TARGET_LINK_LIBRARIES(${REGISTREE_TEST_LIB_NAME} ${REGISTRY_TEST_LIB_NAME})

FILE(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                       "rsc/debug/*.cpp"
                       "rsc/logging/*.cpp"
                       "rsc/config/*.cpp"
                       "rsc/math/*.cpp"
                       "rsc/threading/*.cpp"
                       "rsc/runtime/*.cpp"
                       "rsc/patterns/*.cpp")
SET(TEST_SOURCES ${TEST_SOURCES} "rsc/RscTestSuite.cpp")
LIST(APPEND TEST_SOURCES "rsc/misc/UUIDTest.cpp" "rsc/misc/RegistryTest.cpp" "rsc/misc/langutilsTest.cpp")
LIST(APPEND TEST_SOURCES "rsc/plugins/PluginTest.cpp")

# subprocess
IF(UNIX)
    SET(TEST_SOURCES ${TEST_SOURCES} rsc/subprocess/UnixSubprocessTest.cpp)
ELSEIF(WIN32)
    SET(TEST_SOURCES ${TEST_SOURCES} rsc/subprocess/WindowsSubprocessTest.cpp)
ENDIF()

ADD_EXECUTABLE(${RSC_TEST_NAME} ${TEST_SOURCES})
TARGET_LINK_LIBRARIES(${RSC_TEST_NAME}
                      ${REGISTRY_TEST_LIB_NAME}
                      ${REGISTREE_TEST_LIB_NAME}
                      ${RSC_NAME}
                      ${GMOCK_LIBRARIES})

ADD_TEST(${RSC_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${RSC_TEST_NAME} "--gtest_output=xml:${TEST_RESULT_DIR}/")

# test plugins
SET(TESTPLUGIN_NAMES testplugin
                     testplugin-a-comPlex42NAME
                     testplugin-with.dots.in.name.or.so.version.0.9)
FOREACH(TESTPLUGIN_NAME ${TESTPLUGIN_NAMES})
    ADD_LIBRARY(${TESTPLUGIN_NAME} SHARED rsc/plugins/testplugin.cpp)
    TARGET_LINK_LIBRARIES(${TESTPLUGIN_NAME} ${Boost_LIBRARIES})
    SET_TARGET_PROPERTIES(${TESTPLUGIN_NAME}
                          PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TEST_PLUGIN_DIRECTORY})
ENDFOREACH()

# Pkg-config test
IF(UNIX)
    CONFIGURE_FILE(pkg-config-test.sh.in "${CMAKE_CURRENT_BINARY_DIR}/pkg-config-test.sh")
    ADD_TEST(${PKGCONFIG_TEST_NAME} "${CMAKE_CURRENT_BINARY_DIR}/pkg-config-test.sh")
ENDIF()

IF(WIN32)
    SET(PATH_STRING "$ENV{PATH};${Boost_LIBRARY_DIRS}")
    # requried for PATH entries with a slash before the semicolon
    STRING(REPLACE "\\;" ";" PATH_STRING "${PATH_STRING}")
    STRING(REPLACE ";" "\\;" PATH_STRING "${PATH_STRING}")
    SET_PROPERTY(TEST ${RSC_TEST_NAME}
                 PROPERTY ENVIRONMENT "PATH=${PATH_STRING}")
ENDIF()
