cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.paxos)

include(ExternalProject)

ExternalProject_Add(
    paxos
    GIT_REPOSITORY https://github.com/dgkimura/paxos.git
    INSTALL_COMMAND ""
)

set(paxos_LIB ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/lib PARENT_SCOPE)
set(paxos_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos/src ${CMAKE_CURRENT_BINARY_DIR}paxos-prefix/src/paxos-build/dependencies/boost/include PARENT_SCOPE)
set(paxos_DEPENDENCIES
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/lib/${CMAKE_STATIC_LIBRARY_PREFIX}boost_serialization${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/lib/${CMAKE_STATIC_LIBRARY_PREFIX}boost_system${CMAKE_SHARED_LIBRARY_SUFFIX}
    PARENT_SCOPE)
