#pragma once

#include <functional>

#include "messages.hpp"


namespace dafs
{
    using MessageHandler = std::function<void(dafs::Message message)>;


    class Dispatcher
    {
    public:

        Dispatcher();

        void RouteMessage(dafs::Message);
    };
}
