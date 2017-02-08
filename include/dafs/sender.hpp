#pragma once

#include <mutex>

#include <boost/asio.hpp>

#include "dafs/messages.hpp"


namespace dafs
{
    class Sender
    {
    public:

        virtual void Reply(dafs::Message message) = 0;
    };


    class NetworkSender : public Sender
    {
    public:

        NetworkSender();

        ~NetworkSender();

        void Reply(dafs::Message message);

    private:

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::socket socket;

        std::mutex mutex;
    };
}
