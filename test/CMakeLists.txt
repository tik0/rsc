
ENABLE_TESTING()

SET(TEST_RESULT_DIR ${CMAKE_BINARY_DIR}/testresults)

INCLUDE_DIRECTORIES(BEFORE ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
CONFIGURE_FILE(testconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.h)

# --- library test ---

SET(TEST_SOURCES rsc/RscTestSuite.cpp
                 rsc/logging/ConsoleLoggerTest.cpp
                 rsc/logging/LoggerFactoryTest.cpp
                 rsc/logging/LoggerTest.cpp
                 rsc/misc/RegistryTest.cpp
                 rsc/misc/UUIDTest.cpp
                 rsc/threading/OrderedQueueDispatcherPoolTest.cpp
                 rsc/threading/SynchronizedQueueTest.cpp
                 rsc/threading/TaskTest.cpp)
                 
# subprocess
IF(UNIX)
    SET(TEST_SOURCES ${TEST_SOURCES} rsc/subprocess/UnixSubprocessTest.cpp)
ELSEIF(WIN32)
    SET(TEST_SOURCES ${TEST_SOURCES} rsc/subprocess/WindowsSubprocessTest.cpp)
ENDIF()

ADD_EXECUTABLE(${RSC_TEST_NAME} ${TEST_SOURCES})
TARGET_LINK_LIBRARIES(${RSC_TEST_NAME}
                      ${RSC_NAME}
                      ${Boost_LIBRARIES}
                      ${GMOCK_LIBRARY_NAME})

ADD_TEST(${RSC_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${RSC_TEST_NAME} "--gtest_output=xml:${TEST_RESULT_DIR}/")

IF(WIN32)
    SET(PATH_STRING "$ENV{PATH};${Boost_LIBRARY_DIRS}")
    # requried for PATH entries with a slash before the semicolon
    STRING(REPLACE "\\;" ";" PATH_STRING "${PATH_STRING}")
    STRING(REPLACE ";" "\\;" PATH_STRING "${PATH_STRING}")
    SET_PROPERTY(TEST ${RSC_TEST_NAME}
                 PROPERTY ENVIRONMENT "PATH=${PATH_STRING}")
ENDIF()
