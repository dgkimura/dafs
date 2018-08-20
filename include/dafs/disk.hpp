#pragma once

#include "dafs/delta.hpp"
#include "dafs/blocks.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    dafs::BlockFormat
    DeleteBlock(dafs::BlockInfo info);


    dafs::BlockFormat
    ReadBlock(dafs::BlockInfo info);


    void
    Write(std::string path, dafs::Delta delta);


    dafs::Delta
    Insert(std::iostream& stream, dafs::BlockInfo item);


    dafs::Delta
    Remove(std::iostream& stream, dafs::BlockInfo item);
}
