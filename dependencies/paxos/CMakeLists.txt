cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.paxos)

include(ExternalProject)

ExternalProject_Add(
    paxos
    GIT_REPOSITORY https://github.com/dgkimura/paxos.git
    INSTALL_COMMAND ""
)

set(paxos_LIB ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/lib PARENT_SCOPE)
set(paxos_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos/src PARENT_SCOPE)
