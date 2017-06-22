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


    template <typename T>
    dafs::Delta Insert(
        std::iostream& stream,
        T item)
    {
        auto original = dafs::Deserialize<dafs::Index<T>>(stream);
        auto newset = original;
        newset.items.push_back(item);

        dafs::Delta delta = dafs::CreateDelta(
            dafs::Serialize(original),
            dafs::Serialize(newset));
        return delta;
    }


    template <typename T>
    dafs::Delta Remove(
        std::iostream& stream,
        T item)
    {
        auto original = dafs::Deserialize<dafs::Index<T>>(stream);
        auto newset = original;
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
            dafs::Serialize(original),
            dafs::Serialize(newset));
        return delta;
    }
}
