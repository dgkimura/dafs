#include "sender.hpp"
#include "serialization.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio/io_service.hpp>


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
        std::vector<std::string> ip_port;
        boost::split(ip_port, message.from, boost::is_any_of(":"));

        tcp::resolver resolver(io_service);
        auto endpoint = resolver.resolve(
                            {
                                ip_port[0],
                                ip_port[1]
                            });
        std::lock_guard<std::mutex> guard(mutex);
        boost::asio::connect(socket, endpoint);

        // 1. serialize message
        std::string message_str = dafs::Serialize(message);

        // 2. write message
        boost::asio::write(socket, boost::asio::buffer(message_str.c_str(), message_str.size()));
    }
}
