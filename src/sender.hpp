#pragma once

#include <mutex>

#include <boost/asio.hpp>

#include "messages.hpp"


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

        boost::asio::io_service io_service_;

        boost::asio::ip::tcp::socket socket_;

        std::mutex mutex;
    };
}
