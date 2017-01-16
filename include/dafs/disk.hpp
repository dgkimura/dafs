#pragma once

#include "dafs/delta.hpp"
#include "dafs/filesystem.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    dafs::BlockFormat
    ReadBlock(dafs::BlockInfo info);


    void
    Write(dafs::BlockInfo info, dafs::Delta delta);


    dafs::Delta
    Set(dafs::BlockInfo info,
        std::string content,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block=ReadBlock);


    template <typename T>
    dafs::Delta Insert(
        dafs::BlockInfo info,
        T item,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block=ReadBlock)
    {
        dafs::BlockFormat block = get_block(info);

        auto newset = dafs::Index<T>();
        if (!block.contents.empty())
        {
            newset = dafs::Deserialize<dafs::Index<T>>(block.contents);
        }
        newset.items.push_back(item);
        dafs::Delta delta = dafs::CreateDelta(
            info.path,
            block.contents,
            dafs::Serialize(dafs::Index<T>(newset)));
        return delta;
    }


    template <typename T>
    dafs::Delta Remove(
        dafs::BlockInfo info,
        T item,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block=ReadBlock)
    {
        dafs::BlockFormat block = get_block(info);

        auto newset = dafs::Deserialize<dafs::Index<T>>(block.contents);
        newset.items.erase
        (
            std::remove_if
            (
                newset.items.begin(),
                newset.items.end(),
                [=](const T& current)
                {
                    return current == item;
                }
            ),
            newset.items.end()
        );
        dafs::Delta delta = dafs::CreateDelta(
            info.path,
            block.contents,
            dafs::Serialize(dafs::Index<T>(newset)));
        return delta;
    }
}
