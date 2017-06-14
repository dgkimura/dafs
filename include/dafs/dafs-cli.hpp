#pragma once

#include <array>
#include <functional>
#include <unordered_map>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "dafs/messages.hpp"
#include "dafs/identity.hpp"


namespace dafs
{
    struct Credentials
    {
         dafs::Address address;
    };


    struct SuperBlock
    {
        dafs::Identity filemap;
    };


    struct FileMap
    {
        dafs::Identity next;

        std::unordered_map<std::string, dafs::Identity> files;
    };


    struct File
    {
        std::string filename;

        std::array<dafs::Identity, 100> blocks;

        dafs::Identity extended;
    };


    template <typename Archive>
    void serialize(Archive& ar, dafs::SuperBlock& obj, const unsigned int version)
    {
        ar & obj.filemap;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::FileMap& obj, const unsigned int version)
    {
        ar & obj.next;
        int size;
        if (Archive::is_loading::value)
        {
            ar & size;
            for (int i=0; i<size; i++)
            {
                std::string filename;
                ar & filename;
                dafs::Identity identity;
                ar & identity;

                obj.files[filename] = identity;
            }
        }
        else
        {
            size = obj.files.size();
            ar & size;
            for (auto i : obj.files)
            {
                ar & i.first;
                ar & i.second;
            }
        }
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::File& obj, const unsigned int version)
    {
        ar & obj.filename;
        int size;

        if (Archive::is_loading::value)
        {
            ar & size;
            for (int i=0; i<size; i++)
            {
                dafs::Identity identity;
                ar & identity;
                obj.blocks[i] = identity;
            }
        }
        else
        {
            size = obj.blocks.size();
            ar & size;
            for (int i=0; i<size; i++)
            {
                ar & obj.blocks[i];
            }
        }
        ar & obj.extended;
    }


    void UploadFile(
        dafs::Address address,
        std::vector<std::string> args);


    void DownloadFile(
        dafs::Address address,
        std::vector<std::string> args);


    void GetNodeDetails(
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
        { "upload", dafs::UploadFile },
        { "download", dafs::DownloadFile },
        { "info", dafs::GetNodeDetails },
        { "join", dafs::AddNodeToCluster },
        { "exit", dafs::RemoveNodeFromCluster },
        { "ab", dafs::ExecuteAllocateBlock },
        { "wb", dafs::ExecuteWriteBlock },
        { "rb", dafs::ExecuteReadBlock },
        { "db", dafs::ExecuteDeleteBlock }
    };
}
