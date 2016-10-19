#include "sender.hpp"
#include "serialization.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio/io_service.hpp>


namespace dafs
{
    using boost::asio::ip::tcp;


    NetworkSender::NetworkSender(NetworkSender& sender)
        : io_service_(),
          socket_(io_service_)
    {
    }


    NetworkSender::NetworkSender()
        : io_service_(),
          socket_(io_service_)
    {
    }


    NetworkSender::~NetworkSender()
    {
        socket_.close();
    }


    void
    NetworkSender::Reply(dafs::Message message)
    {
        std::vector<std::string> ip_port;
        boost::split(ip_port, message.from, boost::is_any_of(":"));

        tcp::resolver resolver(io_service_);
        auto endpoint = resolver.resolve(
                            {
                                ip_port[0],
                                ip_port[1]
                            });
        std::lock_guard<std::mutex> guard(mutex);
        boost::asio::connect(socket_, endpoint);

        // 1. serialize message
        std::string message_str = dafs::Serialize(message);

        // 2. write message
        boost::asio::write(socket_, boost::asio::buffer(message_str.c_str(), message_str.size()));
    }
}
