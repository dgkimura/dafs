#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "dafs/dispatcher.hpp"
#include "dafs/filesystem.hpp"


namespace dafs
{
    class Server
    {
    public:

        Server(
            std::string address,
            short port,
            Dispatcher dispather);

        ~Server();

    private:

        void do_accept();

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::acceptor acceptor;

        boost::asio::ip::tcp::socket socket;

        Dispatcher dispatcher;

        class Session : public boost::enable_shared_from_this<Session>
        {
        public:
            Session(
                boost::asio::ip::tcp::socket socket
            );

            void Start(Dispatcher& dispatcher);

        private:

            void handle_read_message(
                Dispatcher& dispatcher,
                const boost::system::error_code& err);

            boost::asio::streambuf response;

            boost::asio::ip::tcp::socket socket;
        };
    };
}
