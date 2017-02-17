#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "dafs/dispatcher.hpp"
#include "dafs/dafs-server.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"
#include "dafs/server.hpp"


void
SetupPartition(
    std::string directory,
    dafs::Address management_interface,
    dafs::Address replication_interface,
    dafs::Identity identity)
{
    if (!boost::filesystem::exists(directory))
    {
        // write out replicaset file
        boost::filesystem::create_directory(directory);
        boost::filesystem::path replicasetfile(directory);
        replicasetfile /= ReplicasetFilename;
        std::fstream rs(replicasetfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        rs << replication_interface.ip << ":"
           << std::to_string(replication_interface.port) << std::endl;

        // write out identity file
        boost::filesystem::path identityfile(directory);
        identityfile /= IdentityFilename;
        std::fstream id_stream(identityfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        id_stream << dafs::Serialize(identity) << std::endl;

        // write out identity file
        boost::filesystem::path authorfile(directory);
        authorfile /= AuthorFilename;
        std::fstream author_stream(authorfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        author_stream << dafs::Serialize(management_interface) << std::endl;
    }
}


void
SetupNode(std::string settings_file)
{
    // write out node config file
    std::fstream config(settings_file, std::ios::out | std::ios::trunc);
    config << "[partition-minus]" << std::endl;
    config << "port = 8070" << std::endl;
    config << "" << std::endl;

    config << "[partition-zero]" << std::endl;
    config << "port = 8080" << std::endl;
    config << "" << std::endl;

    config << "[partition-plus]" << std::endl;
    config << "port = 8090" << std::endl;
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

    if (cli_vm.count("identity"))
    {
        SetupNode(options.settings_file);
    }

    //
    // Parse the configuration file.
    //
    boost::program_options::options_description desc_settings("Configuration options");
    desc_settings.add_options()
        ("partition-minus.port",
         boost::program_options::value(&options.minus_port),
         "port of the plus partition")
        ("partition-zero.port",
         boost::program_options::value(&options.zero_port),
         "port of the zero partition")
        ("partition-plus.port",
         boost::program_options::value(&options.plus_port),
         "port of the plus partition")
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
    if (cli_vm.count("identity"))
    {
        SetupPartition(
            "p-minus",
            dafs::EmptyAddress(),
            dafs::Address(options.address, options.minus_port),
            dafs::Identity(boost::uuids::to_string(options.identity)) -
            dafs::Identity("00000000-0000-0000-0000-0000000000ff")
        );
        SetupPartition(
            "p-zero",
            dafs::Address(options.address, options.port),
            dafs::Address(options.address, options.zero_port),
            dafs::Identity(boost::uuids::to_string(options.identity))
        );
        SetupPartition(
            "p-plus",
            dafs::EmptyAddress(),
            dafs::Address(options.address, options.plus_port),
            dafs::Identity(boost::uuids::to_string(options.identity)) +
            dafs::Identity("00000000-0000-0000-0000-0000000000ff")
        );
    }

    //
    // Start node and server.
    //
    dafs::Node n(
        dafs::Address(options.address, options.minus_port),
        dafs::Address(options.address, options.zero_port),
        dafs::Address(options.address, options.plus_port));
    dafs::Dispatcher dispatcher(n);

    auto server = boost::make_shared<dafs::Server>(
        options.address,
        options.port,
        dispatcher);

    server->Start();

    for (;;);
}
