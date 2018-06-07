#pragma once

#include <boost/asio.hpp>

#include "dafs/messages.hpp"


namespace dafs
{
    class Sender
    {
    public:

        virtual void Send(dafs::Message message) = 0;

        virtual void Reply(dafs::Message message) = 0;

        virtual dafs::Message Receive() = 0;
    };


    class NetworkSender : public Sender
    {
    public:

        NetworkSender(std::shared_ptr<boost::asio::ip::tcp::socket> rsocket);

        NetworkSender();

        ~NetworkSender();

        virtual void Send(dafs::Message message) override;

        virtual void Reply(dafs::Message message) override;

        virtual dafs::Message Receive() override;

    private:

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::socket socket;

        std::shared_ptr<boost::asio::ip::tcp::socket> ssocket;
    };
}
