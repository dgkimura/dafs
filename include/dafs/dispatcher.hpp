#pragma once

#include <functional>
#include <unordered_map>

#include "dafs/messages.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"


namespace dafs
{
    using MessageHandler = std::function
    <
        void(dafs::Message message, std::shared_ptr<dafs::Sender> sender)
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

        Dispatcher(dafs::Node& node);

        void Process(dafs::Message, std::shared_ptr<dafs::Sender> sender);

    private:

        std::unordered_map
        <
            dafs::MessageType,
            dafs::MessageHandler,
            dafs::MessageTypeHash
        > registered_map;

        dafs::Node& node;
    };
}
