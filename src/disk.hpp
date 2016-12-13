#pragma once

#include "delta.hpp"
#include "filesystem.hpp"
#include "serialization.hpp"


namespace dafs
{
    dafs::BlockFormat
    ReadBlock(dafs::BlockInfo info);


    void
    Write(dafs::BlockInfo info, dafs::Delta delta);


    template <typename T>
    dafs::Delta Set(
        dafs::BlockInfo info,
        T item,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block=ReadBlock)
    {
        dafs::BlockFormat block = get_block(info);

        if (!block.contents.empty())
        {
            T olddata = dafs::Deserialize<T>(block.contents);
            dafs::Delta delta = dafs::CreateDelta(
                info.path,
                dafs::SerializeIntoBlockFormat(olddata),
                dafs::SerializeIntoBlockFormat(item));
            return delta;
        }
        else
        {
            dafs::Delta delta = dafs::CreateDelta(
                info.path,
                "",
                dafs::SerializeIntoBlockFormat(item));
            return delta;
        }
    }


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
            dafs::SerializeIntoBlockFormat(newset));
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
            dafs::SerializeIntoBlockFormat(newset));
        return delta;
    }
}
