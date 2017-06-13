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


    void UploadFile(
        dafs::Address address,
        std::vector<std::string> args);


    void DownloadFile(
        dafs::Address address,
        std::vector<std::string> args);


    void ExecuteAllocateBlock(
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


    void AddNodeToCluster(
        dafs::Address address,
        std::vector<std::string> args);


    void RemoveNodeFromCluster(
        dafs::Address address,
        std::vector<std::string> args);


    std::unordered_map<
        std::string,
        std::function<void(dafs::Address address, std::vector<std::string>)>
    > commands =
    {
        { "info", dafs::GetNodeDetails },
        { "upload", dafs::UploadFile },
        { "download", dafs::DownloadFile },
        { "join", dafs::AddNodeToCluster },
        { "exit", dafs::RemoveNodeFromCluster },
        { "ab", dafs::ExecuteAllocateBlock },
        { "wb", dafs::ExecuteWriteBlock },
        { "rb", dafs::ExecuteReadBlock },
        { "db", dafs::ExecuteDeleteBlock }
    };
}
