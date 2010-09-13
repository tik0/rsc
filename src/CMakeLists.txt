
# --- library ---

MESSAGE(STATUS "INIT_METHOD_ATTRIBUTE_CONSTRUCTOR: ${INIT_METHOD_ATTRIBUTE_CONSTRUCTOR}")
MESSAGE(STATUS "INIT_METHOD_CRT: ${INIT_METHOD_CRT}")

IF(INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)
    SET(INIT_METHOD_NAME RSC_HAVE_INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)
ELSEIF(INIT_METHOD_CRT)
    SET(INIT_METHOD_NAME RSC_HAVE_INIT_METHOD_CRT)
ELSE()
    SET(INIT_METHOD_NAME RSC_HAVE_NO_INIT_METHOD_ERROR)
ENDIF()
CONFIGURE_FILE(rsc/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h DESTINATION include/rsc)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

SET(SOURCES rsc/logging/ConsoleLogger.cpp
            rsc/logging/ConsoleLoggingSystem.cpp
            rsc/logging/Logger.cpp
            rsc/logging/LoggerFactory.cpp
            rsc/logging/LoggingSystem.cpp
            rsc/misc/IllegalStateException.cpp
            rsc/misc/langutils.cpp
            rsc/misc/Timer.cpp
            rsc/threading/InterruptedException.cpp
            rsc/threading/OrderedQueueDispatcherPool.cpp
            rsc/threading/SynchronizedQueue.cpp)

SET(HEADERS rsc/logging/ConsoleLogger.h
            rsc/logging/ConsoleLoggingSystem.h
            rsc/logging/Logger.h
            rsc/logging/LoggerFactory.h
            rsc/logging/LoggingSystem.h
            rsc/misc/IllegalStateException.h
            rsc/misc/langutils.h
            rsc/misc/Registry.h
            rsc/misc/Timer.h
            rsc/threading/InterruptedException.h
            rsc/threading/OrderedQueueDispatcherPool.h
            rsc/threading/SynchronizedQueue.h)

ADD_LIBRARY(${RSC_NAME} SHARED ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(${RSC_NAME} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(${RSC_NAME}
                      PROPERTIES
                      VERSION ${RSC_VERSION})

INSTALL(TARGETS ${RSC_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
INSTALL_FILES_RECURSIVE(include HEADERS)
