#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "dafs/dispatcher.hpp"


namespace dafs
{
    class Server : public boost::enable_shared_from_this<Server>
    {
    public:

        Server(
            std::string address,
            short port,
            Dispatcher dispather);

        ~Server();

        void Start();

    private:

        void do_accept();

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::acceptor acceptor;

        Dispatcher dispatcher;

        class Session : public boost::enable_shared_from_this<Session>
        {
        public:
            Session(
                std::shared_ptr<boost::asio::ip::tcp::socket> socket
            );

            void Start(Dispatcher& dispatcher);

        private:

            void handle_read_header(
                Dispatcher& dispatcher,
                const boost::system::error_code& err);

            void handle_read_message(
                Dispatcher& dispatcher,
                const boost::system::error_code& err);

            boost::asio::streambuf response;

            std::shared_ptr<boost::asio::ip::tcp::socket> socket;

            char header[dafs::MessageHeaderSize + 1];
        };
    };
}
