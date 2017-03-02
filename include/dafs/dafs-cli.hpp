#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "dafs/messages.hpp"


namespace dafs
{
    struct Credentials
    {
         dafs::Address address;
    };


    void GetNodeDetails(
        dafs::Address address,
        std::vector<std::string> args);


    void ExecuteWriteBlock(
        dafs::Address address,
        std::vector<std::string> args);


    void ExecuteReadBlock(
        dafs::Address address,
        std::vector<std::string> args);


    void ExecuteDeleteBlock(
        dafs::Address address,
        std::vector<std::string> args);


    void JoinNodeToCluster(
        dafs::Address address,
        std::vector<std::string> args);


    std::unordered_map<
        std::string,
        std::function<void(dafs::Address address, std::vector<std::string>)>
    > commands =
    {
        { "info", dafs::GetNodeDetails },
        { "join", dafs::JoinNodeToCluster },
        { "wb", dafs::ExecuteWriteBlock },
        { "rb", dafs::ExecuteReadBlock },
        { "db", dafs::ExecuteDeleteBlock }
    };
}
