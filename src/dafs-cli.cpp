#include <iostream>
#include <vector>

#include "dafs/dafs-cli.hpp"
#include "dafs/messages.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/sender.hpp"
#include "dafs/serialization.hpp"


dafs::Message
CreateWriteBlockMessage(
    const dafs::Credentials credentials,
    const dafs::BlockInfo info,
    const dafs::BlockFormat format)
{
    return dafs::Message
    {
        credentials.address,    // from
        credentials.address,    // to
        dafs::MessageType::WriteBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize<dafs::BlockInfo>(info)
            },
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::Serialize<dafs::BlockFormat>(format)
            }
        }
    };
}


dafs::Message
CreateReadBlockMessage(
    const dafs::Credentials credentials,
    const dafs::BlockInfo info)
{
    return dafs::Message
    {
        credentials.address,    // from
        credentials.address,    // to
        dafs::MessageType::ReadBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize<dafs::BlockInfo>(info)
            }
        }
    };
}


dafs::Message
CreateGetNodeDetailsMessage(
    const dafs::Credentials credentials,
    const dafs::BlockInfo info)
{
    return dafs::Message
    {
        credentials.address,    // from
        credentials.address,    // to
        dafs::MessageType::GetNodeDetails,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize<dafs::BlockInfo>(info)
            }
        }
    };
}


int main(void)
{
    dafs::Credentials creds
    {
        dafs::Address
        {
            "127.0.0.1",
            9000
        }
    };

    std::string input;

    while (input != "quit")
    {
        std::cout << "dafs> ";
        std::cin >> input;

        if (input == "wb")
        {
            dafs::BlockInfo info
            {
                "path_to_block_info",
                dafs::Identity("11111111-1111-1111-1111-111111111111"),
                0 // revision
            };

            dafs::BlockFormat format
            {
                "this is rawr contents of the block format..."
            };

            dafs::Message m = CreateWriteBlockMessage(creds, info, format);

            auto sender = boost::make_shared<dafs::NetworkSender>(creds.address);
            sender->Send(m);
        }
        else if (input == "rb")
        {
            dafs::BlockInfo info
            {
                "path_to_block_info",
                dafs::Identity("11111111-1111-1111-1111-111111111111"),
                0 // revision
            };
            dafs::Message m = CreateReadBlockMessage(creds, info);

            auto sender = boost::make_shared<dafs::NetworkSender>(creds.address);
            sender->Send(m);
            auto result = sender->Receive();
            std::cout << dafs::Serialize(result);
        }
        else if (input == "info")
        {
            dafs::BlockInfo info
            {
                "path_to_block_info",
                dafs::Identity("11111111-1111-1111-1111-111111111111"),
                0 // revision
            };
            dafs::Message m = CreateGetNodeDetailsMessage(creds, info);

            auto sender = boost::make_shared<dafs::NetworkSender>(creds.address);
            sender->Send(m);
            auto result = sender->Receive();
            auto details = dafs::MetaDataParser(result.metadata)
                           .GetValue<dafs::NodeDetails>(dafs::NodeDetailsKey);

            auto p_minus = details.minus_details;
            auto p_zero = details.zero_details;
            auto p_plus = details.plus_details;
            std::cout << "Node info:\n"
                      << "  - p-minus" << std::endl
                      << "      - author"  << std::endl
                      << "           ip: " << p_minus.author.ip << std::endl
                      << "           port: " << p_minus.author.port << std::endl
                      << "      - identity"  << std::endl
                      << "           uuid: " << p_minus.identity.id << std::endl
                      << "  - p-zero" << std::endl
                      << "      - author"  << std::endl
                      << "           ip: " << p_zero.author.ip << std::endl
                      << "           port: " << p_zero.author.port << std::endl
                      << "      - identity"  << std::endl
                      << "           uuid: " << p_zero.identity.id << std::endl
                      << "  - p-plus" << std::endl
                      << "      - author"  << std::endl
                      << "           ip: " << p_plus.author.ip << std::endl
                      << "           port: " << p_plus.author.port << std::endl
                      << "      - identity"  << std::endl
                      << "           uuid: " << p_plus.identity.id << std::endl;
        }
    }
}
