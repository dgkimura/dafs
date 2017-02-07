#include <boost/asio/io_service.hpp>

#include "dafs/sender.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


    NetworkSender::NetworkSender(const NetworkSender& sender)
        : io_service(),
          socket(io_service)
    {
    }


    NetworkSender::NetworkSender()
        : io_service(),
          socket(io_service)
    {
    }


    NetworkSender::~NetworkSender()
    {
        socket.close();
    }


    void
    NetworkSender::Reply(dafs::Message message)
    {
        tcp::resolver resolver(io_service);
        auto endpoint = resolver.resolve(
                            {
                                message.from.ip,
                                std::to_string(message.from.port)
                            });
        std::lock_guard<std::mutex> guard(mutex);
        boost::asio::connect(socket, endpoint);

        // 1. serialize message
        std::string message_str = dafs::Serialize(message);

        // 2. write message
        boost::asio::write(socket, boost::asio::buffer(message_str.c_str(), message_str.size()));
    }
}
