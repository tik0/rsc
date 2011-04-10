
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

CONFIGURE_FILE(rsc/RSCVersion.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/rsc/RSCVersion.cpp @ONLY)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/rsc)

FILE(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                  "rsc/logging/*.cpp"
                  "rsc/math/*.cpp"
                  "rsc/misc/*.cpp"
                  "rsc/runtime/*.cpp"
                  "rsc/patterns/*.cpp"
                  "rsc/threading/*.cpp")
SET(SOURCES ${SOURCES}
            rsc/subprocess/Subprocess.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/RSCVersion.cpp)

FILE(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                  "rsc/logging/*.h"
                  "rsc/math/*.h"
                  "rsc/misc/*.h"
                  "rsc/runtime/*.h"
                  "rsc/patterns/*.h"
                  "rsc/threading/*.h")
SET(HEADERS ${HEADERS} rsc/subprocess/Subprocess.h)

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
