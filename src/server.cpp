#include "messages.hpp"
#include "serialization.hpp"
#include "server.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


    Server::Server(Dispatcher dispatcher, std::string address, short port)
        : io_service(),
          acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
          socket(io_service),
          dispatcher(dispatcher)
    {
    }


    Server::~Server()
    {
        io_service.stop();
        acceptor.close();
    }


    void
    Server::do_accept()
    {
        acceptor.async_accept(socket,
            [this](boost::system::error_code ec_accept)
            {
                if (!ec_accept)
                {
                    std::make_shared<Session>(std::move(socket))->Start(dispatcher);
                }
                do_accept();
            });
    }


    Server::Session::Session(
        boost::asio::ip::tcp::socket socket
    )
        : socket(std::move(socket))
    {
    }


    void
    Server::Session::Start(Dispatcher& dispatcher)
    {
        auto self(shared_from_this());

        char data_[max_length];

        socket.async_read_some(boost::asio::buffer(data_, max_length),
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
