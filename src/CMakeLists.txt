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
                  "rsc/runtime/*.cpp"
                  "rsc/patterns/*.cpp"
                  "rsc/threading/*.cpp"
                  "rsc/plugins/*.cpp")
SET(SOURCES ${SOURCES}
            rsc/subprocess/Subprocess.cpp

            rsc/debug/DebugTools.cpp

            rsc/misc/langutils.cpp
            rsc/misc/IllegalStateException.cpp
            rsc/misc/UnsupportedOperationException.cpp
            rsc/misc/UUID.cpp

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
                  "rsc/plugins/*.h"
                  "rsc/os/*.h")
SET(HEADERS ${HEADERS}
            rsc/debug/DebugTools.h
            rsc/subprocess/Subprocess.h
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/Version.h
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/config.h
            ${CMAKE_CURRENT_BINARY_DIR}/rsc/rscexports.h)

# Platform-specific backend choices
MESSAGE(STATUS "Selecting platform-specific backends")
IF(CMAKE_SYSTEM_NAME STREQUAL Linux)
    MESSAGE(STATUS "  UnixSubprocess")
    LIST(APPEND SOURCES rsc/subprocess/UnixSubprocess.cpp)
    LIST(APPEND HEADERS rsc/subprocess/UnixSubprocess.h)
    ADD_DEFINITIONS(-DSUBPROCESS_UNIX)

    MESSAGE(STATUS "  LinuxDebugTools")
    LIST(APPEND SOURCES rsc/debug/LinuxDebugTools.cpp)
    LIST(APPEND HEADERS rsc/debug/LinuxDebugTools.h)
    ADD_DEFINITIONS(-DDEBUGTOOLS_LINUX)

    MESSAGE(STATUS "  PosixSignalWaiter")
    LIST(APPEND SOURCES rsc/misc/PosixSignalWaiter.cpp)

    MESSAGE(STATUS "  LinuxProcessInfo")
    LIST(APPEND SOURCES rsc/os/LinuxProcessInfo.cpp)

    MESSAGE(STATUS "  LinuxHostInfo")
    LIST(APPEND SOURCES rsc/os/LinuxHostInfo.cpp)

ELSEIF(UNIX)
    MESSAGE(STATUS "  UnixSubprocess")
    LIST(APPEND SOURCES rsc/subprocess/UnixSubprocess.cpp)
    LIST(APPEND HEADERS rsc/subprocess/UnixSubprocess.h)
    ADD_DEFINITIONS(-DSUBPROCESS_UNIX)

    MESSAGE(STATUS "  LinuxDebugTools")
    LIST(APPEND SOURCES rsc/debug/LinuxDebugTools.cpp)
    LIST(APPEND HEADERS rsc/debug/LinuxDebugTools.h)
    ADD_DEFINITIONS(-DDEBUGTOOLS_LINUX)

    MESSAGE(STATUS "  PosixSignalWaiter")
    LIST(APPEND SOURCES rsc/misc/PosixSignalWaiter.cpp)

    MESSAGE(STATUS "  PosixProcessInfo")
    LIST(APPEND SOURCES rsc/os/PosixProcessInfo.cpp)

    MESSAGE(STATUS "  PosixHostInfo")
    LIST(APPEND SOURCES rsc/os/PosixHostInfo.cpp)

ELSEIF(WIN32)
    MESSAGE(STATUS "  WindowsSubprocess")
    LIST(APPEND SOURCES rsc/subprocess/WindowsSubprocess.cpp)
    LIST(APPEND HEADERS rsc/subprocess/WindowsSubprocess.h)
    ADD_DEFINITIONS(-DSUBPROCESS_WINDOWS)

    MESSAGE(STATUS "  WindowsDebugTools")
    LIST(APPEND SOURCES rsc/debug/WindowsDebugTools.cpp)
    LIST(APPEND HEADERS rsc/debug/WindowsDebugTools.h)
    ADD_DEFINITIONS(-DDEBUGTOOLS_WINDOWS)

    MESSAGE(STATUS "  Win32ProcessInfo")
    LIST(APPEND SOURCES rsc/os/Win32ProcessInfo.cpp)

    MESSAGE(STATUS "  Win32HostInfo")
    LIST(APPEND SOURCES rsc/os/Win32HostInfo.cpp)

ELSE()
    MESSAGE(FATAL_ERROR "No Subprocess, DebugTools, ProcessInfo or HostInfo implementation is available for this platform")
ENDIF()

ADD_LIBRARY(${RSC_NAME} SHARED ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(${RSC_NAME} ${Boost_LIBRARIES}
                                  ${CMAKE_THREAD_LIBS_INIT})
# System libraries for dynamic loading (required by plugin system)
IF(UNIX)
    TARGET_LINK_LIBRARIES(${RSC_NAME} dl)
ENDIF()
# Windows-specific libraries
IF(WIN32)
    TARGET_LINK_LIBRARIES(${RSC_NAME} Secur32)
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

INCLUDE(InstallDebugSymbols)
INSTALL_DEBUG_SYMBOLS(TARGETS ${RSC_NAME})
