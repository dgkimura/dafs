#pragma once

#include "allocator.hpp"
#include "metadata.hpp"
#include "storage.hpp"


namespace dafs
{
    void HandleCreateFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleDeleteFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleOpenFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleReadFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleWriteFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleReadBlock(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleWriteBlock(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleAllocate(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);


    void HandleAllocated(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata);
}
