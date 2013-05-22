# --- library ---

IF(INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)
    SET(INIT_METHOD_NAME RSC_HAVE_INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)
ELSEIF(INIT_METHOD_CRT)
    SET(INIT_METHOD_NAME RSC_HAVE_INIT_METHOD_CRT)
ELSE()
    SET(INIT_METHOD_NAME RSC_HAVE_NO_INIT_METHOD_ERROR)
ENDIF()
STRING(REPLACE "." "_" RSC_EXPORTS_NAME ${RSC_NAME})
CONFIGURE_FILE(rsc/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h DESTINATION "include/${INSTALL_PATH_PREFIX}/rsc")

CONFIGURE_FILE(rsc/rscexports.h.in ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h DESTINATION "include/${INSTALL_PATH_PREFIX}/rsc")

# version stuff
INCLUDE(PadString)
PAD_STRING(MAJOR_PADDED 2 "0" ${RSC_VERSION_MAJOR})
PAD_STRING(MINOR_PADDED 2 "0" ${RSC_VERSION_MINOR})
PAD_STRING(PATCH_PADDED 2 "0" ${RSC_VERSION_PATCH})
SET(RSC_VERSION_NUMERIC "${MAJOR_PADDED}${MINOR_PADDED}${PATCH_PADDED}")
CONFIGURE_FILE(rsc/Version.cpp.in "${CMAKE_CURRENT_BINARY_DIR}/rsc/Version.cpp" @ONLY)
CONFIGURE_FILE(rsc/Version.h.in "${CMAKE_CURRENT_BINARY_DIR}/rsc/Version.h" @ONLY)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/rsc)

FILE(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                  "rsc/logging/*.cpp"
                  "rsc/config/*.cpp"
                  "rsc/math/*.cpp"
                  "rsc/misc/*.cpp"
                  "rsc/runtime/*.cpp"
                  "rsc/patterns/*.cpp"
                  "rsc/threading/*.cpp"
                  "rsc/plugins/*.cpp")
SET(SOURCES ${SOURCES}
            rsc/debug/DebugTools.cpp
            rsc/subprocess/Subprocess.cpp
            "${CMAKE_CURRENT_BINARY_DIR}/rsc/Version.cpp")

FILE(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                  "rsc/logging/*.h"
                  "rsc/config/*.h"
                  "rsc/math/*.h"
                  "rsc/misc/*.h"
                  "rsc/runtime/*.h"
                  "rsc/patterns/*.h"
                  "rsc/patterns/detail/*.h"
                  "rsc/threading/*.h"
                  "rsc/plugins/*.h")
SET(HEADERS ${HEADERS}
            rsc/debug/DebugTools.h
            rsc/subprocess/Subprocess.h
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/Version.h
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h)

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

# debug tools selection
IF(UNIX)
    MESSAGE(STATUS "Selected LinuxDebugTools")
    SET(SOURCES ${SOURCES} rsc/debug/LinuxDebugTools.cpp)
    SET(HEADERS ${HEADERS} rsc/debug/LinuxDebugTools.h)
    ADD_DEFINITIONS(-DDEBUGTOOLS_LINUX)
ELSEIF(WIN32)
    MESSAGE(STATUS "Selected WindowsDebugTools")
    SET(SOURCES ${SOURCES} rsc/debug/WindowsDebugTools.cpp)
    SET(HEADERS ${HEADERS} rsc/debug/WindowsDebugTools.h)
    ADD_DEFINITIONS(-DDEBUGTOOLS_WINDOWS)
ENDIF()

ADD_LIBRARY(${RSC_NAME} SHARED ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(${RSC_NAME} ${Boost_LIBRARIES}
                                  ${CMAKE_THREAD_LIBS_INIT})
# System libraries for dynamic loading (required by plugin system)
IF(UNIX)
    TARGET_LINK_LIBRARIES(${RSC_NAME} dl)
ENDIF()
SET_TARGET_PROPERTIES(${RSC_NAME}
                      PROPERTIES
                      VERSION ${SO_VERSION})

INSTALL(TARGETS ${RSC_NAME}
        EXPORT RSCDepends
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
INSTALL_FILES_RECURSIVE("include/${INSTALL_PATH_PREFIX}" HEADERS)
