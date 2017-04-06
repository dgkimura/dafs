#include <chrono>
#include <future>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "dafs/constants.hpp"
#include "dafs/dispatcher.hpp"
#include "dafs/dafs-server.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"
#include "dafs/server.hpp"


void
CreateReplicatedFile(
    std::string directory,
    std::string filename,
    std::string content)
{
    boost::filesystem::path replicated_file(directory);
    replicated_file /= filename;
    std::fstream replicated_file_stream(
        replicated_file.string(),
        std::ios::in | std::ios::out | std::ios::trunc);
    replicated_file_stream << content << std::endl;
}


void
SetupReplicatedFiles(
    std::string directory,
    dafs::Address management_interface,
    dafs::Address replication_interface,
    dafs::Identity identity,
    dafs::ReplicatedEndpoints details)
{
    if (!boost::filesystem::exists(directory))
    {
        boost::filesystem::create_directory(directory);

        // write out replicaset file
        CreateReplicatedFile(
            directory,
            ReplicasetFilename,
            replication_interface.ip + ":" + std::to_string(
                replication_interface.port)
        );

        // write out identity file
        CreateReplicatedFile(
            directory,
            Constant::IdentityName,
            dafs::Serialize(identity)
        );

        // write out author file
        CreateReplicatedFile(
            directory,
            Constant::AuthorName,
            dafs::Serialize(management_interface)
        );

        // write out details file
        CreateReplicatedFile(
            directory,
            Constant::DetailsName,
            dafs::Serialize(details)
        );
    }
}


std::shared_ptr<dafs::ReplicatedPartition>
SetupPartition(
    std::string directory,
    dafs::Address management_interface,
    dafs::Address replication_interface,
    dafs::Identity identity,
    dafs::ReplicatedEndpoints details,
    std::chrono::seconds ping_interval)
{
    SetupReplicatedFiles(
        directory,
        management_interface,
        replication_interface,
        identity,
        details);

    return std::make_shared<dafs::ReplicatedPartition>(
        replication_interface,
        dafs::Root(directory),
        ping_interval);
}


void
SetupNode(std::string settings_file)
{
    //[partition-minus]
    //port = 8070
    //ping-interval-in-seconds = 5
    //
    //[partition-zero]
    //port = 8080
    //ping-interval-in-seconds = 5
    //
    //[partition-plus]
    //port = 8090
    //ping-interval-in-seconds = 5
    // write out node config file
    std::fstream config(settings_file, std::ios::out | std::ios::trunc);

    config << "[partition-minus]" << std::endl;
    config << "port = 8070" << std::endl;
    config << "ping-interval-in-seconds = 5" << std::endl;
    config << "" << std::endl;

    config << "[partition-zero]" << std::endl;
    config << "port = 8080" << std::endl;
    config << "ping-interval-in-seconds = 5" << std::endl;
    config << "" << std::endl;

    config << "[partition-plus]" << std::endl;
    config << "port = 8090" << std::endl;
    config << "ping-interval-in-seconds = 5" << std::endl;
}


int main(int argc, char** argv)
{
    Options options;

    //
    // Parse the command line options.
    //
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("identity",
         boost::program_options::value(&options.identity),
         "uuid identity of the node")
        ("address",
         boost::program_options::value(&options.address)->default_value("127.0.0.1"),
         "address of the node")
        ("port",
         boost::program_options::value(&options.port)->default_value(9000),
         "port of the node")
        ("config",
         boost::program_options::value(&options.settings_file)->default_value("config.ini"),
         "node configuration file")
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

    //
    // Parse the configuration file.
    //
    boost::program_options::options_description desc_settings("Configuration options");
    desc_settings.add_options()
        ("partition-minus.port",
         boost::program_options::value(&options.minus_port),
         "port of the minus partition")
        ("partition-zero.port",
         boost::program_options::value(&options.zero_port),
         "port of the zero partition")
        ("partition-plus.port",
         boost::program_options::value(&options.plus_port),
         "port of the plus partition")
        ("partition-minus.ping-interval-in-seconds",
         boost::program_options::value(&options.minus_ping_interval)->default_value(3),
         "ping interval of the minus partition")
        ("partition-zero.ping-interval-in-seconds",
         boost::program_options::value(&options.zero_ping_interval)->default_value(3),
         "ping interval of the zero partition")
        ("partition-plus.ping-interval-in-seconds",
         boost::program_options::value(&options.plus_ping_interval)->default_value(3),
         "ping interval of the plus partition")
    ;

    std::ifstream config(options.settings_file);
    boost::program_options::variables_map config_vm;
    boost::program_options::store(
        boost::program_options::parse_config_file<char>(
            options.settings_file.c_str(),
            desc_settings),
        config_vm);
    boost::program_options::notify(config_vm);

    //
    // Setup the partitions
    //
    auto pminus = SetupPartition(
        Constant::PartitionMinusName,
        dafs::EmptyAddress(),
        dafs::Address(options.address, options.minus_port),
        dafs::Identity(boost::uuids::to_string(options.identity)) -
        dafs::Identity("00000000-0000-0000-0000-0000000000ff"),
        dafs::ReplicatedEndpoints
        {
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::EmptyAddress()
            },
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::EmptyAddress()
            },
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::Address(options.address, options.minus_port)
            }
        },
        std::chrono::seconds(options.minus_ping_interval)
    );
    auto pzero = SetupPartition(
        Constant::PartitionZeroName,
        dafs::Address(options.address, options.port),
        dafs::Address(options.address, options.zero_port),
        dafs::Identity(boost::uuids::to_string(options.identity)),
        dafs::ReplicatedEndpoints
        {
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::EmptyAddress()
            },
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::Address(options.address, options.zero_port)
            },
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::EmptyAddress()
            }
        },
        std::chrono::seconds(options.zero_ping_interval)
    );
    auto pplus = SetupPartition(
        Constant::PartitionPlusName,
        dafs::EmptyAddress(),
        dafs::Address(options.address, options.plus_port),
        dafs::Identity(boost::uuids::to_string(options.identity)) +
        dafs::Identity("00000000-0000-0000-0000-0000000000ff"),
        dafs::ReplicatedEndpoints
        {
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::Address(options.address, options.plus_port)
            },
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::EmptyAddress()
            },
            dafs::Endpoint
            {
                dafs::Address(options.address, options.port),
                dafs::EmptyAddress()
            }
        },
        std::chrono::seconds(options.plus_ping_interval)
    );

    //
    // Start node and server.
    //
    dafs::Node n(pminus, pzero, pplus);
    dafs::Dispatcher dispatcher(n);

    auto server = boost::make_shared<dafs::Server>(
        options.address,
        options.port,
        dispatcher);

    server->Start();
    std::promise<void>().get_future().wait();
}
