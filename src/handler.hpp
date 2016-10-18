#pragma once

#include "allocator.hpp"
#include "metadata.hpp"
#include "sender.hpp"
#include "storage.hpp"


namespace dafs
{
    void HandleCreateFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleDeleteFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleOpenFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleReadFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleWriteFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleReadBlock(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleWriteBlock(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleAllocate(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);


    void HandleAllocated(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata,
        dafs::Sender& sender);
}
