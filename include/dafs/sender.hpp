#pragma once

#include <mutex>

#include <boost/asio.hpp>

#include "dafs/messages.hpp"


namespace dafs
{
    class Sender
    {
    public:

        virtual void Send(dafs::Message message) = 0;

        virtual dafs::Message Receive() = 0;
    };


    class NetworkSender : public Sender
    {
    public:

        NetworkSender();

        ~NetworkSender();

        virtual void Send(dafs::Message message) override;

        virtual dafs::Message Receive() override;

    private:

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::socket socket;

        std::mutex mutex;
    };
}
