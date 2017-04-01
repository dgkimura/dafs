#include <cstring>

#include "dafs/messages.hpp"
#include "dafs/serialization.hpp"
#include "dafs/server.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


    Server::Server(
        std::string address,
        short port,
        Dispatcher dispatcher)
        : io_service(),
          acceptor(
              io_service,
              tcp::endpoint(
                  boost::asio::ip::address::from_string(address), port)),
          socket(io_service),
          dispatcher(dispatcher)
    {
        do_accept();
    }


    Server::~Server()
    {
        io_service.stop();
        acceptor.close();
    }


    void
    Server::Start()
    {
        auto self(shared_from_this());
        std::thread([this, self]() { io_service.run(); }).detach();
    }


    void
    Server::do_accept()
    {
        acceptor.async_accept(socket,
            [this](boost::system::error_code ec_accept)
            {
                if (!ec_accept)
                {
                    boost::make_shared<Session>(std::move(socket))->Start(
                        dispatcher);
                }
                do_accept();
            });
    }


    Server::Session::Session(
        boost::asio::ip::tcp::socket socket
    )
        : socket(std::move(socket))
    {
        std::memset(header, 0, dafs::MessageHeaderSize);
        header[dafs::MessageHeaderSize] = '\0';
    }


    void
    Server::Session::Start(Dispatcher& dispatcher)
    {
        // Read header
        boost::asio::async_read(socket,
            boost::asio::buffer(&header, dafs::MessageHeaderSize),
            boost::asio::transfer_at_least(dafs::MessageHeaderSize),
            boost::bind(
                &Session::handle_read_header,
                shared_from_this(),
                dispatcher,
                boost::asio::placeholders::error));
    }


    void
    Server::Session::handle_read_header(
        Dispatcher& dispatcher,
        const boost::system::error_code& err)
    {
        // Read message
        boost::asio::async_read(socket, response,
            boost::asio::transfer_at_least(1),
            boost::bind(
                &Session::handle_read_message,
                shared_from_this(),
                dispatcher,
                boost::asio::placeholders::error));
    }


    void
    Server::Session::handle_read_message(
        Dispatcher& dispatcher,
        const boost::system::error_code& err)
    {
        if (!err && response.size() < std::atoi(header))
        {
            boost::asio::streambuf::const_buffers_type bufs = response.data();
            std::string content(
                boost::asio::buffers_begin(bufs),
                boost::asio::buffers_begin(bufs) + response.size());
            boost::asio::async_read(socket, response,
                boost::asio::transfer_at_least(1),
                boost::bind(
                    &Session::handle_read_message,
                    shared_from_this(),
                    dispatcher,
                    boost::asio::placeholders::error));
        }
        else
        {
            boost::asio::streambuf::const_buffers_type bufs = response.data();
            std::string content(
                boost::asio::buffers_begin(bufs),
                boost::asio::buffers_begin(bufs) + response.size());

            // 1. Deserialize message
            dafs::Message m = Deserialize<Message>(content);

            // 2. Route message to dispatcher for handling.
            auto result = dispatcher.Process(m);

            // 3. Return result to the client.
            std::string result_str = dafs::Serialize(result);
            boost::asio::write(
                socket,
                boost::asio::buffer(result_str.c_str(), result_str.size()));
            socket.close();
        }
    }
}
