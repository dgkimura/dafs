#include "dafs/messages.hpp"
#include "dafs/serialization.hpp"
#include "dafs/server.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


    Server::Server(
        std::string address,
        short port,
        Node node,
        Dispatcher dispatcher)
        : io_service(),
          acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
          socket(io_service),
          node(node),
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
                    std::make_shared<Session>(std::move(socket))->Start(
                        node,
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
    }


    void
    Server::Session::Start(Node& node, Dispatcher& dispatcher)
    {
        boost::asio::async_read(socket, response,
            boost::asio::transfer_at_least(1),
            boost::bind(
                &Session::handle_read_message,
                shared_from_this(),
                node,
                dispatcher,
                boost::asio::placeholders::error));
    }


    void
    Server::Session::handle_read_message(
        Node& node,
        Dispatcher& dispatcher,
        const boost::system::error_code& err)
    {
        if (!err)
        {
            boost::asio::async_read(socket, response,
                boost::asio::transfer_at_least(1),
                boost::bind(
                    &Session::handle_read_message,
                    shared_from_this(),
                    node,
                    dispatcher,
                    boost::asio::placeholders::error));
        }
        if (err == boost::asio::error::eof)
        {
            boost::asio::streambuf::const_buffers_type bufs = response.data();
            std::string content(
                boost::asio::buffers_begin(bufs),
                boost::asio::buffers_begin(bufs) + response.size());

            // 1. Deserialize std::string(data_)
            dafs::Message m = Deserialize<Message>(content);

            // 2. Route message to dispatcher for handling.
            auto routine = dispatcher.GetRoutine(m);
        }
    }
}
