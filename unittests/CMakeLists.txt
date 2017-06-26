cmake_minimum_required(VERSION 3.4)
project(dafs.unittests)

include(ExternalProject)

ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/release-1.7.0.zip
    URL_HASH SHA256=b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
)

set(SOURCES
    blocks_unittest.cpp
    components_unittest.cpp
    customhash_unittest.cpp
    delta_unittest.cpp
    details_unittest.cpp
    disk_unittest.cpp
    handler_unittest.cpp
    identity_unittest.cpp
    metadataparser_unittest.cpp
    node_unittest.cpp
    serialization_unittest.cpp
    signal_unittest.cpp
)

add_executable(all_unittests ${SOURCES})

set_property(TARGET all_unittests PROPERTY CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if (UNIX AND NOT APPLE)
    # XXX: GCC link order matters. Ensure that 'rt' links after 'pthread'.
    set(PLATFORM_LIBRARIES rt)
endif()

target_link_libraries(all_unittests
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/gtest-prefix/src/gtest-build/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CMAKE_CURRENT_BINARY_DIR}/gtest-prefix/src/gtest-build/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${paxos_LIBS}
        ${diflib_LIB}
        Threads::Threads
        ${PLATFORM_LIBRARIES}
        dafs-library
)

add_dependencies(all_unittests gtest)
add_dependencies(all_unittests diflib)
add_dependencies(all_unittests paxos)
add_dependencies(all_unittests dafs-library)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest-prefix/src/gtest/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${diflib_HEADERS})
include_directories(${paxos_HEADERS})

set(CMAKE_INSTALL_RPATH ${paxos_LIBS} ${diflib_LIB})
