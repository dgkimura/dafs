#include <iostream>
#include <vector>

#include "dafs/messages.hpp"
#include "dafs/sender.hpp"


int main(void)
{
    std::string input;

    while (input != "quit")
    {
        std::cout << "dafs> ";
        std::cin >> input;

        dafs::Message m
        {
            "127.0.0.1:9000",       // from
            "127.0.0.1:9000",       // to
            "my content",
            dafs::MessageType::WriteBlock,
            std::vector<dafs::MetaData>()
        };

        dafs::NetworkSender sender;
        sender.Reply(m);
    }
}
