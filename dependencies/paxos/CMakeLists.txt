cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.paxos)

include(ExternalProject)

set(PAXOS_SOURCES https://github.com/dgkimura/paxos/archive/master.zip CACHE STRING "Default path to paxos sources")

ExternalProject_Add(
    paxos
    URL ${PAXOS_SOURCES}
    INSTALL_COMMAND ""
)

set(paxos_DEPENDENCIES
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_filesystem${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_iostreams${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_log${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_program_options${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_serialization${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_system${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_thread${CMAKE_SHARED_LIBRARY_SUFFIX}
)
set(paxos_LIBS
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/lib/${CMAKE_SHARED_LIBRARY_PREFIX}paxos${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${paxos_DEPENDENCIES}
    PARENT_SCOPE)
set(paxos_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos/include
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/include
    PARENT_SCOPE)
