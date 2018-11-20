#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "dafs/dafs-cli.hpp"
#include "dafs/details.hpp"
#include "dafs/messages.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/sender.hpp"


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
    InitializeFileService(
        dafs::Address address,
        std::vector<std::string> args)
    {
        ExecuteWriteBlock(
            address,
            dafs::BlockInfo
            {
                "00000000-0000-0000-0000-000000000000",
                dafs::Identity("00000000-0000-0000-0000-000000000000"),
            },
            dafs::BlockFormat
            {
                dafs::Serialize(
                    dafs::SuperBlock
                    {
                        ExecuteAllocateBlock(address).identity
                    }
                )
            }
        );
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

        //
        // Get the superblock.
        //
        auto superblock = dafs::Deserialize<dafs::SuperBlock>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    "00000000-0000-0000-0000-000000000000",
                    dafs::Identity("00000000-0000-0000-0000-000000000000"),
                }
            ).contents
        );

        //
        // Get the file metadata map.
        //
        auto metadata_map = dafs::Deserialize<dafs::FileMetadataMap>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    superblock.filemap.id,
                    superblock.filemap
                }
            ).contents
        );

        //
        // Create file metadata if it does not exist already.
        //
        if (metadata_map.files.find(filename) == metadata_map.files.end())
        {
            auto blockinfo = ExecuteAllocateBlock(address);
            metadata_map.files[filename] = blockinfo.identity;

            // Update metadata map.
            ExecuteWriteBlock(
                address,
                dafs::BlockInfo
                {
                    superblock.filemap.id,
                    superblock.filemap
                },
                dafs::BlockFormat
                {
                    dafs::Serialize(metadata_map)
                }
            );

            // Update file metadata.
            ExecuteWriteBlock(
                address,
                dafs::BlockInfo
                {
                    metadata_map.files[filename].id,
                    metadata_map.files[filename]
                },
                dafs::BlockFormat
                {
                    dafs::Serialize(FileMetadata{})
                }
            );
        }

        //
        // Get file metadata.
        //
        auto metadata = dafs::Deserialize<dafs::FileMetadata>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    metadata_map.files[filename].id,
                    metadata_map.files[filename]
                }
            ).contents
        );

        //
        // Allocate and write blocks.
        //
        std::fstream file((boost::filesystem::path(filename)).string(),
                          std::ios::in | std::ios::binary);

        dafs::FileParser parser(file);
        dafs::BlockFormat format = parser.Next();

        for (int i=0; !format.contents.empty(); i+= 1)
        {
            auto blockinfo = ExecuteAllocateBlock(address);
            ExecuteWriteBlock(
                address,
                dafs::BlockInfo
                {
                    blockinfo.identity.id,
                    blockinfo.identity
                },
                format
            );

            metadata.blocks[i] = blockinfo.identity;
            format = parser.Next();
        }

        //
        // Update file metadata.
        //
        ExecuteWriteBlock(
            address,
            dafs::BlockInfo
            {
                metadata_map.files[filename].id,
                metadata_map.files[filename]
            },
            dafs::BlockFormat
            {
                dafs::Serialize(metadata)
            }
        );
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

        //
        // Get the superblock.
        //
        auto superblock = dafs::Deserialize<dafs::SuperBlock>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    "00000000-0000-0000-0000-000000000000",
                    dafs::Identity("00000000-0000-0000-0000-000000000000"),
                }
            ).contents
        );

        //
        // Get the file metadata map.
        //
        auto metadata_map = dafs::Deserialize<dafs::FileMetadataMap>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    superblock.filemap.id,
                    superblock.filemap
                }
            ).contents
        );

        //
        // Get file metadata.
        //
        auto metadata = dafs::Deserialize<dafs::FileMetadata>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    metadata_map.files[filename].id,
                    metadata_map.files[filename]
                }
            ).contents
        );

        std::fstream file((boost::filesystem::path(filename)).string(),
                          std::ios::out | std::ios::binary);
        for (const auto& identity : metadata.blocks)
        {
            if (identity == dafs::Identity())
            {
                break;
            }

            file << ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    identity.id,
                    identity
                }
            ).contents;
        }
    }


    void
    RemoveFile(
        dafs::Address address,
        std::vector<std::string> args)
    {
        if (args.size() < 2)
        {
            std::cout << "Command requires filename.\n";
            return;
        }

        std::string filename = args[1];
        std::cout << "Removing file: " << filename << std::endl;

        //
        // Get the superblock.
        //
        auto superblock = dafs::Deserialize<dafs::SuperBlock>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    "00000000-0000-0000-0000-000000000000",
                    dafs::Identity("00000000-0000-0000-0000-000000000000"),
                }
            ).contents
        );

        //
        // Get the file metadata map.
        //
        auto metadata_map = dafs::Deserialize<dafs::FileMetadataMap>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    superblock.filemap.id,
                    superblock.filemap
                }
            ).contents
        );

        //
        // Get file metadata.
        //
        auto metadata = dafs::Deserialize<dafs::FileMetadata>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    metadata_map.files[filename].id,
                    metadata_map.files[filename]
                }
            ).contents
        );

        for (const auto& identity : metadata.blocks)
        {
            if (identity == dafs::Identity())
            {
                break;
            }

            ExecuteDeleteBlock(
                address,
                dafs::BlockInfo
                {
                    identity.id,
                    identity
                }
            );
        }

        //
        // Delete file metadata
        //
        ExecuteDeleteBlock(
            address,
            dafs::BlockInfo
            {
                metadata_map.files[filename].id,
                metadata_map.files[filename]
            }
        );

        //
        // Update file metadata map.
        //
        metadata_map.files.erase(filename);
        ExecuteWriteBlock(
            address,
            dafs::BlockInfo
            {
                superblock.filemap.id,
                superblock.filemap
            },
            dafs::BlockFormat
            {
                dafs::Serialize(metadata_map)
            }
        );
    }


    void
    ListFiles(
        dafs::Address address,
        std::vector<std::string> args)
    {
        //
        // Get the superblock.
        //
        auto superblock = dafs::Deserialize<dafs::SuperBlock>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    "00000000-0000-0000-0000-000000000000",
                    dafs::Identity("00000000-0000-0000-0000-000000000000"),
                }
            ).contents
        );

        //
        // Get the file metadata map.
        //
        auto metadata_map = dafs::Deserialize<dafs::FileMetadataMap>(
            ExecuteReadBlock(
                address,
                dafs::BlockInfo
                {
                    superblock.filemap.id,
                    superblock.filemap
                }
            ).contents
        );

        if (metadata_map.files.size() == 0)
        {
            std::cout << "<empty>" << std::endl;
        }
        for (const auto& i : metadata_map.files)
        {
            std::cout << "    " << i.first << std::endl;
        }
    }


    void
    ListServers(
        dafs::Address address,
        std::vector<std::string> args)
    {
        std::cout << std::left << std::setw(20) << "Management"
                  << std::left << std::setw(20) << "Minus Interface"
                  << std::left << std::setw(20) << "Zero Interface"
                  << std::left << std::setw(20) << "Plus Interface"
                  << std::left << std::setw(20) << "Fault-Domain"
                  << std::left << std::setw(40) << "Identity" << std::endl;

        dafs::Address current = address;
        for (;;)
        {
            auto sender = boost::make_shared<dafs::NetworkSender>();
            sender->Send(
                current,
                dafs::Message
                {
                    dafs::MessageType::GetNodeDetails,
                    std::vector<dafs::MetaData>{}
                }
            );
            auto result = sender->Receive();
            auto parser = dafs::MetaDataParser(result.metadata);

            auto p_minus = parser.GetValue<dafs::ReplicatedEndpoints>(dafs::MinusReplicatedEndpointsKey);
            auto p_zero = parser.GetValue<dafs::ReplicatedEndpoints>(dafs::ZeroReplicatedEndpointsKey);
            auto p_plus = parser.GetValue<dafs::ReplicatedEndpoints>(dafs::PlusReplicatedEndpointsKey);

            std::cout << std::left << std::setw(20) << p_zero.zero.management.ip + ":" + std::to_string(p_zero.zero.management.port);
            std::cout << std::left << std::setw(20) << p_minus.plus.replication.ip + ":" + std::to_string(p_minus.plus.replication.port);
            std::cout << std::left << std::setw(20) << p_zero.zero.replication.ip + ":" + std::to_string(p_zero.zero.replication.port);
            std::cout << std::left << std::setw(20) << p_plus.minus.replication.ip + ":" + std::to_string(p_plus.minus.replication.port);
            std::cout << std::left << std::setw(20) << p_zero.zero.fault_domain;
            std::cout << std::left << std::setw(40) << p_zero.zero.identity.id << std::endl;

            current = p_plus.zero.management;
            if (current.ip == address.ip && current.port == address.port)
            {
                break;
            }
        }
    }


    void
    GetNodeDetails(
        dafs::Address address,
        std::vector<std::string> args)
    {
        auto message = dafs::Message
        {
            dafs::MessageType::GetNodeDetails,
            std::vector<dafs::MetaData>
            {
            }
        };
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(address, message);
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
            << "     - fault-domain"  << std::endl
            << "          " << p_minus.zero.fault_domain << std::endl
            << " - p-zero" << std::endl
            << "     - identity"  << std::endl
            << "         " << p_zero.zero.identity.id << std::endl
            << "     - plus"  << std::endl
            << "          " << EndpointToString(p_zero.plus) << std::endl
            << "     - zero"  << std::endl
            << "          " << EndpointToString(p_zero.zero) << std::endl
            << "     - minus"  << std::endl
            << "          " << EndpointToString(p_zero.minus) << std::endl
            << "     - fault-domain"  << std::endl
            << "          " << p_zero.zero.fault_domain << std::endl
            << " - p-plus" << std::endl
            << "          " << p_plus.zero.identity.id << std::endl
            << "     - plus"  << std::endl
            << "          " << EndpointToString(p_plus.plus) << std::endl
            << "     - zero"  << std::endl
            << "          " << EndpointToString(p_plus.zero) << std::endl
            << "     - minus"  << std::endl
            << "          " << EndpointToString(p_plus.minus) << std::endl
            << "     - fault-domain"  << std::endl
            << "          " << p_plus.zero.fault_domain << std::endl;
    }


    dafs::BlockInfo
    ExecuteAllocateBlock(
        dafs::Address address)
    {
        auto message = dafs::Message
        {
            dafs::MessageType::AllocateBlock,
            std::vector<dafs::MetaData>
            {
            }
        };
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(address, message);
        return dafs::MetaDataParser(sender->Receive().metadata)
                   .GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
    }


    void
    ExecuteWriteBlock(
        dafs::Address address,
        dafs::BlockInfo info,
        dafs::BlockFormat format)
    {
        HardSend(
            address,
            dafs::Message
            {
                dafs::MessageType::WriteBlock,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::BlockInfoKey,
                        dafs::serialize<dafs::BlockInfo>(info)
                    },
                    dafs::MetaData
                    {
                        dafs::BlockFormatKey,
                        dafs::serialize<dafs::BlockFormat>(format)
                    }
                }
            }
        );
    }


    dafs::BlockFormat
    ExecuteReadBlock(
        dafs::Address address,
        dafs::BlockInfo info)
    {
        dafs::Message result = HardSend(
            address,
            dafs::Message
            {
                dafs::MessageType::ReadBlock,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::BlockInfoKey,
                        dafs::serialize<dafs::BlockInfo>(info)
                    }
                }
            }
        );
        return dafs::MetaDataParser(result.metadata)
                   .GetValue<dafs::BlockFormat>(dafs::BlockFormatKey);
    }


    void
    ExecuteDeleteBlock(
        dafs::Address address,
        dafs::BlockInfo info)
    {
        HardSend(
            address,
            dafs::Message
            {
                dafs::MessageType::DeleteBlock,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::BlockInfoKey,
                        dafs::serialize<dafs::BlockInfo>(info)
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
            dafs::MessageType::_JoinCluster,
            std::vector<dafs::MetaData>
            {
                dafs::MetaData
                {
                    dafs::AddressKey,
                    dafs::serialize<dafs::Address>(
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
        sender->Send(address, message);
        sender->Wait();
    }


    void
    RemoveNodeFromCluster(
        dafs::Address address,
        std::vector<std::string> args)
    {
        auto message = dafs::Message
        {
            dafs::MessageType::ExitCluster,
            std::vector<dafs::MetaData>
            {
            }
        };
        std::cout << "RemoveNodeFromCluster: ip:" << address.ip << " port:"<< address.port<<"\n";
        auto sender = boost::make_shared<dafs::NetworkSender>();
        sender->Send(address, message);
    }


    void
    DisplayCommands(
        dafs::Address address,
        std::vector<std::string> args)
    {
        std::cout << "List of available commands:" << std::endl;
        for (auto c : commands)
        {
            std::cout << "    " << c.first << std::endl;
        }
    }


    dafs::Message
    HardSend(dafs::Address address, dafs::Message message)
    {
        dafs::Message result;
        do
        {
            auto sender = boost::make_shared<dafs::NetworkSender>();
            sender->Send(address, message);
            result = sender->Receive();
            if (result.type == dafs::MessageType::Failure)
            {
                address = dafs::MetaDataParser(result.metadata)
                                   .GetValue<dafs::Address>(dafs::AddressKey);
            }
        } while(result.type == dafs::MessageType::Failure);
        return result;
    }


    FileParser::FileParser(std::iostream& filestream)
        : stream(filestream)
    {
    }


    dafs::BlockFormat
    FileParser::Next()
    {
        std::memset(buffer, '\0', dafs::BLOCK_SIZE_IN_BYTES);
        stream.read(buffer, dafs::BLOCK_SIZE_IN_BYTES);

        dafs::BlockFormat format;
        format.contents = std::string(buffer);
        format.contents[dafs::BLOCK_SIZE_IN_BYTES] = '\0';
        return format;
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

    std::string input;

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("address",
         boost::program_options::value(&creds.address.ip)->default_value("127.0.0.1"),
         "address of the node")
        ("port",
         boost::program_options::value(&creds.address.port)->default_value(9000),
         "port of the node")
        ("command",
         boost::program_options::value(&input)->default_value(""),
         "command to be run")
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

    if (!input.empty())
    {
        std::vector<std::string> tokens;
        boost::split(tokens, input, boost::is_any_of("\t "));

        dafs::commands[tokens[0]](creds.address, tokens);
        std::exit(EXIT_SUCCESS);
    }

    while (std::cout << "dafs@" << creds.address.ip << ":" << creds.address.port << "> " && std::getline(std::cin, input) && input != "quit")
    {
        std::vector<std::string> tokens;
        boost::split(tokens, input, boost::is_any_of("\t "));

        dafs::commands[tokens[0]](creds.address, tokens);
    }
}
