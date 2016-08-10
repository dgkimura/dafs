cmake_minimum_required(VERSION 3.4)
project(dafs.dependencies)

add_subdirectory(paxos)

set(paxos_LIB ${paxos_LIB} PARENT_SCOPE)
set(paxos_HEADERS ${paxos_HEADERS} PARENT_SCOPE)
set(paxos_DEPENDENCIES ${paxos_DEPENDENCIES} PARENT_SCOPE)
