cmake_minimum_required(VERSION 3.4)
project(dafs.src)

set(SOURCES
    blocks.cpp
    commit.cpp
    components.cpp
    delta.cpp
    details.cpp
    disk.cpp
    dispatcher.cpp
    handler.cpp
    identity.cpp
    metadataparser.cpp
    node.cpp
    partition.cpp
    propose.cpp
    replication.cpp
    sender.cpp
    serialization.cpp
    server.cpp
    signal.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if (UNIX AND NOT APPLE)
    # XXX: GCC link order matters. Ensure that 'rt' links after 'pthread'.
    set(PLATFORM_LIBRARIES rt)
endif()

add_library(dafs-library ${SOURCES})
target_link_libraries(dafs-library
    PRIVATE
        ${paxos_LIBS}
        ${diflib_LIB}
        Threads::Threads
        ${PLATFORM_LIBRARIES}
)
add_dependencies(dafs-library diflib)
add_dependencies(dafs-library paxos)
add_dependencies(dafs-library dafs-proto)

add_executable(dafs-server dafs-server.cpp)
target_link_libraries(dafs-server
    PRIVATE
        dafs-library
        dafs-proto
        dependency-protobuf
)
add_dependencies(dafs-server dafs-library)
add_dependencies(dafs-server dafs-proto)

add_executable(dafs-cli dafs-cli.cpp)
target_link_libraries(dafs-cli
    PRIVATE
        dafs-library
        dafs-proto
        dependency-protobuf
)
add_dependencies(dafs-cli dafs-library)
add_dependencies(dafs-cli dafs-proto)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${diflib_HEADERS})
include_directories(${paxos_HEADERS})
include_directories(${proto_HEADERS})
include_directories(${protobuf_HEADERS})

set_property(TARGET dafs-library PROPERTY CXX_STANDARD 11)
set_property(TARGET dafs-server PROPERTY CXX_STANDARD 11)
set_property(TARGET dafs-cli PROPERTY CXX_STANDARD 11)

set(CMAKE_INSTALL_RPATH ${paxos_LIBS} ${diflib_LIB})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${CMAKE_CURRENT_BINARY_DIR}/config.ini COPYONLY)
