cmake_minimum_required(VERSION 3.4)
project(dafs.src)

set(SOURCES
    commit.cpp
    delta.cpp
    disk.cpp
    dispatcher.cpp
    filesystem.cpp
    handler.cpp
    metadataparser.cpp
    node.cpp
    partition.cpp
    propose.cpp
    loader.cpp
    sender.cpp
    server.cpp
    signal.cpp
    replicated.cpp
)
add_executable(dafs-server main.cpp ${SOURCES})
add_executable(dafs-cli prompt.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if (UNIX AND NOT APPLE)
    # XXX: GCC link order matters. Ensure that 'rt' links after 'pthread'.
    set(PLATFORM_LIBRARIES rt)
endif()

target_link_libraries(dafs-server
    PRIVATE
        ${paxos_LIBS}
        ${diflib_LIB}
        Threads::Threads
        ${PLATFORM_LIBRARIES}
)

add_dependencies(dafs-server diflib)
add_dependencies(dafs-server paxos)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${diflib_HEADERS})
include_directories(${paxos_HEADERS})

set_property(TARGET dafs-server PROPERTY CXX_STANDARD 11)

foreach(source ${SOURCES})
    set(DAFS_SOURCES ${DAFS_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
endforeach()
set(DAFS_SOURCES ${DAFS_SOURCES} PARENT_SCOPE)

set(CMAKE_INSTALL_RPATH ${paxos_LIBS} ${diflib_LIB})
