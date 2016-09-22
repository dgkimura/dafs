cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies)

add_subdirectory(paxos)
add_subdirectory(diflib)

set(paxos_LIB ${paxos_LIB} PARENT_SCOPE)
set(paxos_HEADERS ${paxos_HEADERS} PARENT_SCOPE)
set(paxos_DEPENDENCIES ${paxos_DEPENDENCIES} PARENT_SCOPE)

set(diflib_LIB ${diflib_LIB} PARENT_SCOPE)
set(diflib_HEADERS ${diflib_HEADERS} PARENT_SCOPE)
