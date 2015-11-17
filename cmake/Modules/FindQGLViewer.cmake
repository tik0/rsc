# - Try to find QGLViewer
# Once done this will define
#
#  QGLVIEWER_FOUND - system has QGLViewer
#  QGLVIEWER_INCLUDE_DIR - the QGLViewer include directory
#  QGLVIEWER_LIBRARIES - Link these to use QGLViewer
#  QGLVIEWER_DEFINITIONS - Compiler switches required for using QGLViewer
#

MESSAGE(STATUS "QGLVIEWERROOT is: \"${QGLVIEWERROOT}:$ENV{QGLVIEWERROOT}\"")

FIND_PATH(QGLVIEWER_INCLUDE_DIR
          NAMES QGLViewer/qglviewer.h
          PATHS ${QGLVIEWERROOT}
                $ENV{QGLVIEWERROOT}
                /usr/include
                /usr/local/include)

IF(QGLVIEWER_INCLUDE_DIR)
	MESSAGE(STATUS "Found QglViewer include-dir: ${QGLVIEWER_INCLUDE_DIR}")
ELSE()
	MESSAGE(STATUS "Could NOT find QglViewer include-dir.")
ENDIF()


FIND_LIBRARY(QGLVIEWER_LIBRARY_RELEASE
             NAMES qglviewer-qt4 qglviewer QGLViewer QGLViewer2
             PATHS ${QGLVIEWERROOT}
                   $ENV{QGLVIEWERROOT}
                   ENV LD_LIBRARY_PATH
                   ENV LIBRARY_PATH
		           /usr/lib
                   /usr/local/lib
             PATH_SUFFIXES QGLViewer QGLViewer/release)

IF(QGLVIEWER_LIBRARY_RELEASE)
	MESSAGE(STATUS "Found QglViewer library (release): ${QGLVIEWER_LIBRARY_RELEASE}")
ELSE()
	MESSAGE(STATUS "Could NOT find QglViewer library (release).")
ENDIF()

FIND_LIBRARY(QGLVIEWER_LIBRARY_DEBUG
             NAMES dqglviewer dQGLViewer dQGLViewer2
             PATHS ${QGLVIEWERROOT}
                   $ENV{QGLVIEWERROOT}
                   ENV LD_LIBRARY_PATH
                   ENV LIBRARY_PATH
		           /usr/lib
                   /usr/local/lib
             PATH_SUFFIXES QGLViewer QGLViewer/debug)

IF(QGLVIEWER_LIBRARY_DEBUG)
	MESSAGE(STATUS "Found QglViewer library (debug): ${QGLVIEWER_LIBRARY_DEBUG}")
ELSE()
	MESSAGE(STATUS "Could NOT find QglViewer library (debug).")
ENDIF()

IF(QGLVIEWER_LIBRARY_RELEASE)
    IF(QGLVIEWER_LIBRARY_DEBUG)
        SET(QGLVIEWER_LIBRARIES_ optimized ${QGLVIEWER_LIBRARY_RELEASE} debug ${QGLVIEWER_LIBRARY_DEBUG})
    ELSE()
        SET(QGLVIEWER_LIBRARIES_ ${QGLVIEWER_LIBRARY_RELEASE})
    ENDIF()

    SET(QGLVIEWER_LIBRARIES ${QGLVIEWER_LIBRARIES_} CACHE FILEPATH "The QGLViewer library")
ENDIF()

IF(QGLVIEWER_INCLUDE_DIR AND QGLVIEWER_LIBRARIES)
   SET(QGLVIEWER_FOUND TRUE)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQGLVIEWER_FOUND")
ENDIF()

IF(QGLVIEWER_FOUND)
    IF(NOT QGLViewer_FIND_QUIETLY)
        MESSAGE(STATUS "Found QGLViewer: ${QGLVIEWER_LIBRARIES}")
    ENDIF()
ELSE(QGLVIEWER_FOUND)
    IF(QGLViewer_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find QGLViewer")
    ENDIF()
ENDIF()

