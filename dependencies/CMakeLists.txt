cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies)

add_subdirectory(paxos)
add_subdirectory(diflib)

set(paxos_LIBS ${paxos_LIBS} PARENT_SCOPE)
set(paxos_HEADERS ${paxos_HEADERS} PARENT_SCOPE)

set(diflib_LIB ${diflib_LIB} PARENT_SCOPE)
set(diflib_HEADERS ${diflib_HEADERS} PARENT_SCOPE)
