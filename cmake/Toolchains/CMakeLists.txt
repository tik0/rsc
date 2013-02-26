
IF(INSTALL_TOOLCHAINS)

    MESSAGE(STATUS "Installing CMake toolchain files to: ${RSC_CMAKE_TOOLCHAIN_PATH}")
    
    INSTALL(FILES Oncilla_RB110.cmake
            DESTINATION ${RSC_CMAKE_TOOLCHAIN_PATH}
            COMPONENT dev)

ENDIF()
