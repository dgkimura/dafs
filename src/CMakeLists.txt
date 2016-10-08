cmake_minimum_required(VERSION 3.4)
project(dafs.src)

set(SOURCES
    blockcache.cpp
    delta.cpp
    dispatcher.cpp
    handler.cpp
    messages.cpp
    server.cpp
    storage.cpp
    topology.cpp
)
add_executable(dafs main.cpp ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if (UNIX AND NOT APPLE)
    # XXX: GCC link order matters. Ensure that 'rt' links after 'pthread'.
    set(PLATFORM_LIBRARIES rt)
endif()

target_link_libraries(dafs
    PRIVATE
        ${paxos_LIBS}
        ${diflib_LIB}
        Threads::Threads
        ${PLATFORM_LIBRARIES}
)

add_dependencies(dafs diflib)
add_dependencies(dafs paxos)

include_directories(${diflib_HEADERS})
include_directories(${paxos_HEADERS})

set_property(TARGET dafs PROPERTY CXX_STANDARD 11)

set(dafs_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

foreach(source ${SOURCES})
    set(DAFS_SOURCES ${DAFS_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
endforeach()
set(DAFS_SOURCES ${DAFS_SOURCES} PARENT_SCOPE)

set(CMAKE_INSTALL_RPATH ${paxos_LIBS} ${diflib_LIB})
