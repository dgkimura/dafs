#pragma once

#include "allocator.hpp"
#include "storage.hpp"


namespace dafs
{
    void HandleCreateFile(
        dafs::Storage store,
        dafs::FileInfo info);


    void HandleDeleteFile(
        dafs::Storage store,
        dafs::FileInfo info);


    void HandleOpenFile(
        dafs::Storage store,
        dafs::FileInfo info);


    void HandleReadFile(
        dafs::Storage store,
        dafs::FileInfo info);


    void HandleWriteFile(
        dafs::Bytes bytes,
        dafs::Storage store,
        dafs::FileInfo info);


    void HandleAllocate(
        dafs::Allocation allocation,
        dafs::Storage store);


    void HandleAllocated(
        dafs::Allocation allocation,
        dafs::Storage store);
}
