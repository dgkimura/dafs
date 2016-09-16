#include <boost/asio.hpp>

#include "filesystem.hpp"
#include "storage.hpp"


namespace dafs
{
    const std::string FileNameTag = "FileName";

    class Server
    {
    public:

        Server(Storage storage, std::string address, short port);

        ~Server();

    private:

        void do_accept();

        boost::asio::io_service io_service_;

        boost::asio::ip::tcp::acceptor acceptor_;

        boost::asio::ip::tcp::socket socket_;

        Storage storage;

        class Session : public std::enable_shared_from_this<Session>
        {
        public:
            Session(
                boost::asio::ip::tcp::socket socket
            );

            void Start(Storage& storage);

        private:

            boost::asio::ip::tcp::socket socket_;

            enum {max_length = 8192};
        };
    };
}
