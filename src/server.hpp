#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "dispatcher.hpp"
#include "filesystem.hpp"
#include "node.hpp"


namespace dafs
{
    class Server
    {
    public:

        Server(
            std::string address,
            short port,
            Node node,
            Dispatcher dispather);

        ~Server();

    private:

        void do_accept();

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::acceptor acceptor;

        boost::asio::ip::tcp::socket socket;

        Node node;

        Dispatcher dispatcher;

        class Session : public boost::enable_shared_from_this<Session>
        {
        public:
            Session(
                boost::asio::ip::tcp::socket socket
            );

            void Start(Node& node, Dispatcher& dispatcher);

        private:

            void handle_read_message(
                Node& node,
                Dispatcher& dispatcher,
                const boost::system::error_code& err);

            boost::asio::streambuf response;

            boost::asio::ip::tcp::socket socket;
        };
    };
}
