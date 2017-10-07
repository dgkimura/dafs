#include <boost/asio/io_service.hpp>

#include "dafs/sender.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    using boost::asio::ip::tcp;


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
    NetworkSender::Send(dafs::Message message)
    {
        try
        {
            tcp::resolver resolver(io_service);
            auto endpoint = resolver.resolve(
                                {
                                    message.to.ip,
                                    std::to_string(message.to.port)
                                });
            boost::asio::connect(socket, endpoint);

            // 1. serialize message
            std::string message_data = dafs::Serialize(message);
            std::stringstream ss;
            ss << std::setw(dafs::MessageHeaderSize)
               << std::to_string(message_data.size());
            std::string message_header = ss.str();

            std::string message_full = message_header + message_data;

            // 2. write message
            boost::asio::write(
                socket,
                boost::asio::buffer(message_full.c_str(), message_full.size()));
        }
        catch (boost::system::system_error& e)
        {
            // TODO: log an error here
        }
    }


    dafs::Message
    NetworkSender::Receive()
    {
        boost::system::error_code error;
        boost::asio::streambuf reply;
        while (boost::asio::read(
            socket,
            reply,
            boost::asio::transfer_at_least(1),
            error));

        boost::asio::streambuf::const_buffers_type bufs = reply.data();
        std::string content(
            boost::asio::buffers_begin(bufs),
            boost::asio::buffers_begin(bufs) + reply.size());
        return dafs::Deserialize<dafs::Message>(content);
    }
}
