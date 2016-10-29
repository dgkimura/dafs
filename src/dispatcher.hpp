#pragma once

#include <functional>
#include <unordered_map>

#include "messages.hpp"
#include "metadataparser.hpp"
#include "sender.hpp"
#include "storage.hpp"


namespace dafs
{
    using MessageHandler = std::function
    <
        void (
            dafs::Storage& store,
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

        Dispatcher(dafs::Storage& storage);

        Dispatcher(dafs::Dispatcher& dispatcher);

        void RouteMessage(dafs::Message);

    private:

        std::unordered_map
        <
            dafs::MessageType,
            dafs::MessageHandler,
            dafs::MessageTypeHash
        > registered_map;

        dafs::Storage& store;

        dafs::NetworkSender sender;
    };
}
