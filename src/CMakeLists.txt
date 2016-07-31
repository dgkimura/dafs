cmake_minimum_required(VERSION 3.4)
project(dafs.src)

set(SOURCES
    dafs.cpp
)

add_executable(dafs ${SOURCES})
add_dependencies(dafs paxos)

set_property(TARGET dafs PROPERTY CXX_STANDARD 11)

target_link_libraries(dafs
    PRIVATE
        ${paxos_LIB}/${CMAKE_SHARED_LIBRARY_PREFIX}paxos${CMAKE_SHARED_LIBRARY_SUFFIX}
)
include_directories(${paxos_HEADERS})
