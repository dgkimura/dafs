#include "serialization.hpp"
#include "server.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


    Server::Server(Storage storage_, std::string address, short port)
        : io_service_(),
          acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
          socket_(io_service_),
          storage(storage_)
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
                    std::make_shared<Session>(std::move(socket_))->Start(storage);
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
    Server::Session::Start(Storage& storage)
    {
        auto self(shared_from_this());

        char data_[max_length];

        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [self, &data_, &storage](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    // 1. Deserialize std::string(data_)
                    Message m = Deserialize<Message>(data_);

                    // 2. Read request
                    // 3. Find block
                    // 4. Return response
                }
            }
        );
    }
}
