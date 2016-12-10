#pragma once

#include "delta.hpp"
#include "filesystem.hpp"
#include "serialization.hpp"


namespace dafs
{
    dafs::BlockFormat
    ReadBlock(dafs::BlockInfo info);


    void
    WriteBlock(dafs::BlockInfo info, dafs::Delta delta);


    template <typename T>
    dafs::Delta IndexAdd(
        dafs::BlockInfo info,
        T item,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block=ReadBlock)
    {
        dafs::BlockFormat block = get_block(info);

        auto oldset = dafs::Index<T>();
        auto newset = dafs::Index<T>();
        if (!block.contents.empty())
        {
            oldset = dafs::Deserialize<dafs::Index<T>>(block.contents);
            newset = dafs::Deserialize<dafs::Index<T>>(block.contents);
        }
        newset.items.push_back(item);
        dafs::Delta delta = dafs::CreateDelta(
            info.path,
            dafs::SerializeIntoBlockFormat(oldset),
            dafs::SerializeIntoBlockFormat(newset));
        return delta;
    }


    template <typename T>
    dafs::Delta IndexRemove(
        dafs::BlockInfo info,
        T item,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block=ReadBlock)
    {
        dafs::BlockFormat block = get_block(info);

        auto oldset = dafs::Deserialize<dafs::Index<T>>(block.contents);
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
            dafs::SerializeIntoBlockFormat(oldset),
            dafs::SerializeIntoBlockFormat(newset));
        return delta;
    }
}
