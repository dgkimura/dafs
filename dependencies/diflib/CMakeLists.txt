cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.diflib)

include(ExternalProject)

ExternalProject_Add(
    diflib
    GIT_REPOSITORY https://github.com/gdkimura/diflib.git
    INSTALL_COMMAND ""
)

set(diflib_LIB ${CMAKE_CURRENT_BINARY_DIR}/diflib-prefix/src/diflib-build PARENT_SCOPE)
set(diflib_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/diflib-prefix/src/diflib PARENT_SCOPE)
