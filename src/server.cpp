#include "messages.hpp"
#include "serialization.hpp"
#include "server.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


    Server::Server(Dispatcher dispatcher_, std::string address, short port)
        : io_service_(),
          acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
          socket_(io_service_),
          dispatcher(dispatcher_)
    {
    }


    Server::~Server()
    {
        io_service_.stop();
        acceptor_.close();
    }


    void
    Server::do_accept()
    {
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec_accept)
            {
                if (!ec_accept)
                {
                    std::make_shared<Session>(std::move(socket_))->Start(dispatcher);
                }
                do_accept();
            });
    }


    Server::Session::Session(
        boost::asio::ip::tcp::socket socket
    )
        : socket_(std::move(socket))
    {
    }


    void
    Server::Session::Start(Dispatcher& dispatcher)
    {
        auto self(shared_from_this());

        char data_[max_length];

        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [self, &data_, &dispatcher](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    // 1. Deserialize std::string(data_)
                    dafs::Message m = Deserialize<Message>(data_);

                    // 2. Route message to dispatcher for handling.
                    dispatcher.RouteMessage(m);
                }
            }
        );
    }
}
