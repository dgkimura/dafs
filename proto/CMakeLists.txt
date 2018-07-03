cmake_minimum_required(VERSION 3.4)
project(dafs.proto)

add_custom_command(
    OUTPUT message.pb.h message.pb.cc
    COMMAND ${protobuf_BIN}/protoc -I${CMAKE_CURRENT_SOURCE_DIR} message.proto --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/message.proto
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(dafs-proto message.pb.cc)
add_dependencies(dafs-proto protobuf)

include_directories(${protobuf_HEADERS})

set(proto_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}
    PARENT_SCOPE)
