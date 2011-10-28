MESSAGE(STATUS "Loading toolchain for Oncilla system")

# System description
SET(CMAKE_SYSTEM_NAME "Linux")
SET(CMAKE_SYSTEM_PROCESSOR "i586")
SET(UNIX "True")
SET(CMAKE_COMPILER_IS_GNUCXX "True")

# CXX compiler flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i586" CACHE STRING "" FORCE)
