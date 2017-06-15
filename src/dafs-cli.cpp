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
    std::string
    EndpointToString(dafs::Endpoint address)
    {
        std::stringstream result;
        result << "{" << address.management.ip
               << ":" << address.management.port << "}"
               << "{" << address.replication.ip
               << ":" << address.replication.port << "}";
        return result.str();
    }


    void
    UploadFile(
        dafs::Address address,
        std::vector<std::string> args)
    {
        if (args.size() < 2)
        {
            std::cout << "Command requires filename.\n";
            return;
        }

        std::string filename = args[1];
        std::cout << "Uploading file: " << filename << std::endl;
    }


    void
    DownloadFile(
        dafs::Address address,
        std::vector<std::string> args)
    {
        if (args.size() < 2)
        {
            std::cout << "Command requires filename.\n";
            return;
        }

        std::string filename = args[1];
        std::cout << "Downloading file: " << filename << std::endl;
    }


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

        std::cout
            << "Node info:\n"
            << " - p-minus" << std::endl
            << "     - identity"  << std::endl
            << "          " << p_minus.zero.identity.id << std::endl
            << "     - plus"  << std::endl
            << "          " << EndpointToString(p_minus.plus) << std::endl
            << "     - zero"  << std::endl
            << "          " << EndpointToString(p_minus.zero) << std::endl
            << "     - minus"  << std::endl
            << "          " << EndpointToString(p_minus.minus) << std::endl
            << " - p-zero" << std::endl
            << "     - identity"  << std::endl
            << "         " << p_zero.zero.identity.id << std::endl
            << "     - plus"  << std::endl
            << "          " << EndpointToString(p_zero.plus) << std::endl
            << "     - zero"  << std::endl
            << "          " << EndpointToString(p_zero.zero) << std::endl
            << "     - minus"  << std::endl
            << "          " << EndpointToString(p_zero.minus) << std::endl
            << " - p-plus" << std::endl
            << "          " << p_plus.zero.identity.id << std::endl
            << "     - plus"  << std::endl
            << "          " << EndpointToString(p_plus.plus) << std::endl
            << "     - zero"  << std::endl
            << "          " << EndpointToString(p_plus.zero) << std::endl
            << "     - minus"  << std::endl
            << "          " << EndpointToString(p_plus.minus) << std::endl;
    }


    void
    ExecuteAllocateBlock(
        dafs::Address address,
        std::vector<std::string> args)
    {
        auto message = dafs::Message
        {
            address, // from
            address, // to
            dafs::MessageType::AllocateBlock,
            std::vector<dafs::MetaData>
            {
            }
        };
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
        auto result = sender->Receive();
        std::cout << dafs::Serialize(result);
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
        dafs::Message result = HardSend(
            dafs::Message
            {
                address,
                address,
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
            }
        );
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

        dafs::Message result = HardSend(
            dafs::Message
            {
                address,
                address,
                dafs::MessageType::ReadBlock,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::BlockInfoKey,
                        dafs::Serialize<dafs::BlockInfo>(info)
                    }
                }
            }
        );

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

        dafs::Message result = HardSend(
            dafs::Message
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
            }
        );
    }


    void
    AddNodeToCluster(
        dafs::Address address,
        std::vector<std::string> args)
    {
        if (args.size() < 3)
        {
            std::cout << "Command requires address and port of node in cluster.\n";
            return;
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
        std::cout << "AddNodeToCluster: ip:" << address.ip << " port:"<< address.port<<"\n";
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
    }


    void
    RemoveNodeFromCluster(
        dafs::Address address,
        std::vector<std::string> args)
    {
        auto message = dafs::Message
        {
            address, // from
            address, // to
            dafs::MessageType::ExitCluster,
            std::vector<dafs::MetaData>
            {
            }
        };
        std::cout << "RemoveNodeFromCluster: ip:" << address.ip << " port:"<< address.port<<"\n";
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(message);
    }


    dafs::Message
    HardSend(dafs::Message message)
    {
        auto address = message.to;

        dafs::Message result;
        do
        {
            auto sender = boost::make_shared<dafs::NetworkSender>();
            sender->Send(message);
            result = sender->Receive();
            if (result.type == dafs::MessageType::Failure)
            {
                message.to = dafs::MetaDataParser(result.metadata)
                                   .GetValue<dafs::Address>(dafs::AddressKey);
            }
        } while(result.type == dafs::MessageType::Failure);
        return result;
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
