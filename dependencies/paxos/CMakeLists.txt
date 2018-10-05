cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies.paxos)

include(ExternalProject)

if (${PAXOS_SOURCES})
    ExternalProject_Add(
        paxos
        URL ${PAXOS_SOURCES}
        INSTALL_COMMAND ""
    )
else()
    ExternalProject_Add(
        paxos
        GIT_REPOSITORY https://github.com/dgkimura/paxos.git
        GIT_TAG 1.0.13
        INSTALL_COMMAND ""
    )
endif()

set(paxos_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/lib/${CMAKE_SHARED_LIBRARY_PREFIX}paxos${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_atomic${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_chrono${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_date_time${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_regex${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_filesystem${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_iostreams${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_log${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_program_options${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_serialization${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_system${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_thread${CMAKE_SHARED_LIBRARY_SUFFIX}
)
set(paxos_LIBS
    ${paxos_FILES}
    PARENT_SCOPE)
set(paxos_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos/include
    ${CMAKE_CURRENT_BINARY_DIR}/paxos-prefix/src/paxos-build/dependencies/boost/boost-prefix/include
    PARENT_SCOPE)
install(FILES
    ${paxos_FILES}
    DESTINATION lib)
