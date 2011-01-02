
# --- library ---

IF(INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)
    SET(INIT_METHOD_NAME RSC_HAVE_INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)
ELSEIF(INIT_METHOD_CRT)
    SET(INIT_METHOD_NAME RSC_HAVE_INIT_METHOD_CRT)
ELSE()
    SET(INIT_METHOD_NAME RSC_HAVE_NO_INIT_METHOD_ERROR)
ENDIF()
CONFIGURE_FILE(rsc/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h DESTINATION include/rsc)

CONFIGURE_FILE(rsc/rscexports.h.in ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h DESTINATION include/rsc)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

SET(SOURCES rsc/logging/ConsoleLogger.cpp
            rsc/logging/ConsoleLoggingSystem.cpp
            rsc/logging/Logger.cpp
            rsc/logging/LoggerFactory.cpp
            rsc/logging/LoggingSystem.cpp
            rsc/math/MathUtils.cpp
            rsc/math/Matrix.cpp
            rsc/misc/IllegalStateException.cpp
            rsc/misc/langutils.cpp
            rsc/misc/UUID.cpp
            rsc/subprocess/Subprocess.cpp
            rsc/threading/InterruptedException.cpp
            rsc/threading/OrderedQueueDispatcherPool.cpp
            rsc/threading/PeriodicTask.cpp
            rsc/threading/SynchronizedQueue.cpp
            rsc/threading/Task.cpp
            rsc/threading/TaskExecutor.cpp)

SET(HEADERS rsc/logging/ConsoleLogger.h
            rsc/logging/ConsoleLoggingSystem.h
            rsc/logging/Logger.h
            rsc/logging/LoggerFactory.h
            rsc/logging/LoggingSystem.h
            rsc/math/MathUtils.h
            rsc/math/Matrix.h
            rsc/misc/IllegalStateException.h
            rsc/misc/langutils.h
            rsc/misc/Registry.h
            rsc/misc/UUID.h
            rsc/subprocess/Subprocess.h
            rsc/threading/InterruptedException.h
            rsc/threading/PeriodicTask.h
            rsc/threading/OrderedQueueDispatcherPool.h
            rsc/threading/SynchronizedQueue.h
            rsc/threading/Task.h
            rsc/threading/TaskExecutor.h)
            
# subprocess choice
IF(UNIX)
    MESSAGE(STATUS "Selected UnixSubprocess")
    SET(SOURCES ${SOURCES} rsc/subprocess/UnixSubprocess.cpp)
    SET(HEADERS ${HEADERS} rsc/subprocess/UnixSubprocess.h)
    ADD_DEFINITIONS(-DSUBPROCESS_UNIX)
ELSEIF(WIN32)
    MESSAGE(STATUS "Selected WindowsSubprocess")
    SET(SOURCES ${SOURCES} rsc/subprocess/WindowsSubprocess.cpp)
    SET(HEADERS ${HEADERS} rsc/subprocess/WindowsSubprocess.h)
    ADD_DEFINITIONS(-DSUBPROCESS_WINDOWS)
ENDIF()

ADD_LIBRARY(${RSC_NAME} SHARED ${SOURCES} ${HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h)
TARGET_LINK_LIBRARIES(${RSC_NAME} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(${RSC_NAME}
                      PROPERTIES
                      VERSION ${RSC_VERSION})

INSTALL(TARGETS ${RSC_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
INSTALL_FILES_RECURSIVE(include HEADERS)

# --- cppcheck ---

GENERATE_CPPCHECK(SOURCES ${SOURCES}
                          ${HEADERS}
                          ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h
                          ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h
                  SUPPRESSION_FILE "cppcheck-suppressions")
