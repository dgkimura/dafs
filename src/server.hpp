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

        boost::asio::io_service io_service_;

        boost::asio::ip::tcp::acceptor acceptor_;

        boost::asio::ip::tcp::socket socket_;

        Dispatcher dispatcher;

        class Session : public std::enable_shared_from_this<Session>
        {
        public:
            Session(
                boost::asio::ip::tcp::socket socket
            );

            void Start(Dispatcher& dispatcher);

        private:

            boost::asio::ip::tcp::socket socket_;

            enum {max_length = 8192};
        };
    };
}
