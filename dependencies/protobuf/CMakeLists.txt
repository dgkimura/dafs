cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.protobuf)

include(ExternalProject)

set(INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf-prefix")

ExternalProject_Add(
    protobuf
    URL https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-cpp-3.5.1.zip
    CONFIGURE_COMMAND  <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)

add_library(dependency-protobuf STATIC IMPORTED GLOBAL)
set_property(TARGET dependency-protobuf PROPERTY CXX_STANDARD 11)
set_target_properties(dependency-protobuf PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
  IMPORTED_LOCATION "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}")

set(protobuf_BIN
    ${CMAKE_CURRENT_BINARY_DIR}/protobuf-prefix/bin
    PARENT_SCOPE)
set(protobuf_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/protobuf-prefix/include
    PARENT_SCOPE)
