#pragma once

#include <array>
#include <functional>
#include <unordered_map>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/unordered_map.hpp>

#include "dafs/blocks.hpp"
#include "dafs/identity.hpp"
#include "dafs/messages.hpp"
#include "dafs/serialization.hpp"


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


    struct FileMetadataMap
    {
        dafs::Identity next;

        std::unordered_map<std::string, dafs::Identity> files;
    };


    struct FileMetadata
    {
        std::array<dafs::Identity, 100> blocks;

        dafs::Identity extended;
    };


    template <typename Archive>
    void serialize(Archive& ar, dafs::SuperBlock& obj, const unsigned int version)
    {
        ar & obj.filemap;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::FileMetadataMap& obj, const unsigned int version)
    {
        ar & obj.next;
        ar & obj.files;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::FileMetadata& obj, const unsigned int version)
    {
        ar & obj.blocks;
        ar & obj.extended;
    }


    class FileParser
    {
    public:

        FileParser(std::iostream& filestream);

        dafs::BlockFormat Next();

    private:

        std::iostream& stream;

        char buffer[dafs::BLOCK_SIZE_IN_BYTES];
    };


    void InitializeFileService(
        dafs::Address address,
        std::vector<std::string> args);


    void UploadFile(
        dafs::Address address,
        std::vector<std::string> args);


    void DownloadFile(
        dafs::Address address,
        std::vector<std::string> args);


    void RemoveFile(
        dafs::Address address,
        std::vector<std::string> args);


    void ListFiles(
        dafs::Address address,
        std::vector<std::string> args);


    void GetNodeDetails(
        dafs::Address address,
        std::vector<std::string> args);


    dafs::BlockInfo ExecuteAllocateBlock(
        dafs::Address address);


    void ExecuteWriteBlock(
        dafs::Address address,
        dafs::BlockInfo info,
        dafs::BlockFormat format);


    dafs::BlockFormat ExecuteReadBlock(
        dafs::Address address,
        dafs::BlockInfo info);


    void ExecuteDeleteBlock(
        dafs::Address address,
        dafs::BlockInfo info);


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
        { "initialize", dafs::InitializeFileService },
        { "upload", dafs::UploadFile },
        { "download", dafs::DownloadFile },
        { "remove", dafs::RemoveFile },
        { "list-files", dafs::ListFiles },
        { "info", dafs::GetNodeDetails },
        { "join", dafs::AddNodeToCluster },
        { "exit", dafs::RemoveNodeFromCluster }
    };


    dafs::Message HardSend(dafs::Message message);
}
