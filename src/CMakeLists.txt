cmake_minimum_required(VERSION 3.4)
project(dafs.src)

set(SOURCES
    blockcache.cpp
    filesystem.cpp
    operations.cpp
    storage.cpp
    topology.cpp
)
add_executable(dafs main.cpp ${SOURCES})
target_link_libraries(dafs
    PRIVATE
        ${paxos_LIB}/${CMAKE_SHARED_LIBRARY_PREFIX}paxos${CMAKE_SHARED_LIBRARY_SUFFIX}
)

add_dependencies(dafs paxos)
include_directories(${paxos_HEADERS})

set_property(TARGET dafs PROPERTY CXX_STANDARD 11)

set(dafs_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

foreach(source ${SOURCES})
    set(DAFS_SOURCES ${DAFS_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
endforeach()
set(DAFS_SOURCES ${DAFS_SOURCES} PARENT_SCOPE)
