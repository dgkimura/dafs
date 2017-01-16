#pragma once

#include <functional>
#include <unordered_map>

#include "dafs/messages.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/partition.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    using MessageHandler = std::function
    <
        void (
            dafs::Partition& partition,
            dafs::MetaDataParser metadata,
            dafs::Sender& sender)
    >;


    struct MessageTypeHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };


    class Dispatcher
    {
    public:

        Dispatcher();

        MessageHandler GetRoutine(dafs::Message);

    private:

        std::unordered_map
        <
            dafs::MessageType,
            dafs::MessageHandler,
            dafs::MessageTypeHash
        > registered_map;
    };
}
