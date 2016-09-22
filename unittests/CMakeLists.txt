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
    filesystem_unittest.cpp
    messages_unittest.cpp
    operations_unittest.cpp
    serialization_unittest.cpp
    topology_unittest.cpp
)

add_executable(all_unittests ${SOURCES} ${DAFS_SOURCES})
add_dependencies(all_unittests gtest)

set_property(TARGET all_unittests PROPERTY CXX_STANDARD 11)

target_link_libraries(all_unittests
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/gtest-prefix/src/gtest-build/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CMAKE_CURRENT_BINARY_DIR}/gtest-prefix/src/gtest-build/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${PLATFORM_LIBRARIES}
        ${paxos_DEPENDENCIES}
        ${paxos_LIB}/${CMAKE_SHARED_LIBRARY_PREFIX}paxos${CMAKE_SHARED_LIBRARY_SUFFIX}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest-prefix/src/gtest/include)
include_directories(${dafs_DIR})
include_directories(${paxos_HEADERS})
