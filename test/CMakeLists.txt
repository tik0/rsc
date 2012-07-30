ENABLE_TESTING()

SET(TEST_RESULT_DIR ${CMAKE_BINARY_DIR}/testresults)

INCLUDE_DIRECTORIES(BEFORE SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/src)

SET(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
CONFIGURE_FILE(testconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.h)

# --- library test ---

SET(REGISTRY_TEST_LIB_NAME "registry")
ADD_LIBRARY(${REGISTRY_TEST_LIB_NAME} SHARED "rsc/misc/testRegistry.cpp" "rsc/misc/testRegistry.h")

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
LIST(APPEND TEST_SOURCES "rsc/misc/UUIDTest.cpp" "rsc/misc/RegistryTest.cpp")

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
                      ${Boost_LIBRARIES}
                      ${GMOCK_LIBRARIES}
                      ${CMAKE_THREAD_LIBS_INIT})

ADD_TEST(${RSC_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${RSC_TEST_NAME} "--gtest_output=xml:${TEST_RESULT_DIR}/")

IF(WIN32)
    SET(PATH_STRING "$ENV{PATH};${Boost_LIBRARY_DIRS}")
    # requried for PATH entries with a slash before the semicolon
    STRING(REPLACE "\\;" ";" PATH_STRING "${PATH_STRING}")
    STRING(REPLACE ";" "\\;" PATH_STRING "${PATH_STRING}")
    SET_PROPERTY(TEST ${RSC_TEST_NAME}
                 PROPERTY ENVIRONMENT "PATH=${PATH_STRING}")
ENDIF()
