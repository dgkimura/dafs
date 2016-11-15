#pragma once

#include <boost/asio.hpp>

#include "filesystem.hpp"
#include "dispatcher.hpp"


namespace dafs
{
    class Server
    {
    public:

        Server(Dispatcher dispather, std::string address, short port);

        ~Server();

    private:

        void do_accept();

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::acceptor acceptor;

        boost::asio::ip::tcp::socket socket;

        Dispatcher dispatcher;

        class Session : public std::enable_shared_from_this<Session>
        {
        public:
            Session(
                boost::asio::ip::tcp::socket socket
            );

            void Start(Dispatcher& dispatcher);

        private:

            boost::asio::ip::tcp::socket socket;

            enum {max_length = 8192};
        };
    };
}
