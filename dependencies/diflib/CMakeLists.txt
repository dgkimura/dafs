cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.diflib)

include(ExternalProject)

if(${DIFLIB_SOURCES})
    ExternalProject_Add(
        diflib
        URL ${DIFLIB_SOURCES}
        INSTALL_COMMAND ""
    )
else()
    ExternalProject_Add(
        diflib
        GIT_REPOSITORY https://github.com/gdkimura/diflib.git
        INSTALL_COMMAND ""
    )
endif()

set(diflib_LIB
    ${CMAKE_CURRENT_BINARY_DIR}/diflib-prefix/src/diflib-build/${CMAKE_SHARED_LIBRARY_PREFIX}diflib${CMAKE_STATIC_LIBRARY_SUFFIX}
    PARENT_SCOPE)
set(diflib_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/diflib-prefix/src/diflib PARENT_SCOPE)
