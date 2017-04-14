#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include "dafs/dafs-cli.hpp"
#include "dafs/messages.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/sender.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    void
    GetNodeDetails(
        dafs::Address address,
        std::vector<std::string> args)
    {
        auto message = dafs::Message
        {
            address, // from
            address, // to
            dafs::MessageType::GetNodeDetails,
            std::vector<dafs::MetaData>
            {
            }
        };
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
        auto result = sender->Receive();
        auto parser = dafs::MetaDataParser(result.metadata);

        auto p_minus = parser.GetValue<dafs::ReplicatedEndpoints>(dafs::MinusReplicatedEndpointsKey);
        auto p_zero = parser.GetValue<dafs::ReplicatedEndpoints>(dafs::ZeroReplicatedEndpointsKey);
        auto p_plus = parser.GetValue<dafs::ReplicatedEndpoints>(dafs::PlusReplicatedEndpointsKey);

        auto p_minus_id = parser.GetValue<dafs::Identity>(dafs::MinusIdentityKey);
        auto p_zero_id = parser.GetValue<dafs::Identity>(dafs::ZeroIdentityKey);
        auto p_plus_id = parser.GetValue<dafs::Identity>(dafs::PlusIdentityKey);

        std::cout << "Node info:\n"
                  << "  - p-minus" << std::endl
                  << "      - author"  << std::endl
                  << "           ip: " << p_minus.zero.management.ip << std::endl
                  << "           port: " << p_minus.zero.management.port << std::endl
                  << "      - identity"  << std::endl
                  << "           uuid: " << p_minus_id.id << std::endl
                  << "  - p-zero" << std::endl
                  << "      - author"  << std::endl
                  << "           ip: " << p_zero.zero.management.ip << std::endl
                  << "           port: " << p_zero.zero.management.port << std::endl
                  << "      - identity"  << std::endl
                  << "           uuid: " << p_zero_id.id << std::endl
                  << "  - p-plus" << std::endl
                  << "      - author"  << std::endl
                  << "           ip: " << p_plus.zero.management.ip << std::endl
                  << "           port: " << p_plus.zero.management.port << std::endl
                  << "      - identity"  << std::endl
                  << "           uuid: " << p_plus_id.id << std::endl;
    }


    void
    ExecuteWriteBlock(
        dafs::Address address,
        std::vector<std::string> args)
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
        auto message = dafs::Message
        {
            address, // from
            address, // to
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
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
    }


    void
    ExecuteReadBlock(
        dafs::Address address,
        std::vector<std::string> args)
    {
        dafs::BlockInfo info
        {
            "path_to_block_info",
            dafs::Identity("11111111-1111-1111-1111-111111111111"),
            0 // revision
        };

        auto message = dafs::Message
        {
            address, // from
            address, // to
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
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
        auto result = sender->Receive();
        std::cout << dafs::Serialize(result);
    }


    void
    ExecuteDeleteBlock(
        dafs::Address address,
        std::vector<std::string> args)
    {
        dafs::BlockInfo info
        {
            "path_to_block_info",
            dafs::Identity("11111111-1111-1111-1111-111111111111"),
            0 // revision
        };

        auto message = dafs::Message
        {
            address, // from
            address, // to
            dafs::MessageType::DeleteBlock,
            std::vector<dafs::MetaData>
            {
                dafs::MetaData
                {
                    dafs::BlockInfoKey,
                    dafs::Serialize<dafs::BlockInfo>(info)
                }
            }
        };
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
    }


    void
    JoinNodeToCluster(
        dafs::Address address,
        std::vector<std::string> args)
    {
        if (args.size() < 3)
        {
            std::cout << "Command requires address and port of node in cluster.\n";
        }

        auto message = dafs::Message
        {
            address, // from
            address, // to
            dafs::MessageType::_JoinCluster,
            std::vector<dafs::MetaData>
            {
                dafs::MetaData
                {
                    dafs::AddressKey,
                    dafs::Serialize<dafs::Address>(
                        dafs::Address
                        {
                            args[1],
                            boost::lexical_cast<short>(args[2])
                        }
                    )
                }
            }
        };
        std::cout << "JoinNodeToCluster: ip:" << address.ip << " port:"<< address.port<<"\n";
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
    }
}


int main(int argc, char** argv)
{
    dafs::Credentials creds
    {
        dafs::Address
        {
            "127.0.0.1",
            9000
        }
    };

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("address",
         boost::program_options::value(&creds.address.ip)->default_value("127.0.0.1"),
         "address of the node")
        ("port",
         boost::program_options::value(&creds.address.port)->default_value(9000),
         "port of the node")
    ;
    boost::program_options::variables_map cli_vm;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc),
        cli_vm);
    boost::program_options::notify(cli_vm);

    if (cli_vm.count("help"))
    {
        std::cout << desc << "\n";
        std::exit(EXIT_FAILURE);
    }

    std::string input;

    while (std::cout << "dafs> " && std::getline(std::cin, input) && input != "quit")
    {
        std::vector<std::string> tokens;
        boost::split(tokens, input, boost::is_any_of("\t "));

        dafs::commands[tokens[0]](creds.address, tokens);
    }
}
