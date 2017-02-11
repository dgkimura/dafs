#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid/uuid.hpp>

#include "dafs/dispatcher.hpp"
#include "dafs/dafs-server.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"
#include "dafs/server.hpp"


void
SetupPartition(std::string directory, std::string hostport, dafs::Identity identity)
{
    if (!boost::filesystem::exists(directory))
    {
        boost::filesystem::create_directory(directory);
        boost::filesystem::path replicasetfile(directory);
        replicasetfile /= ReplicasetFilename;
        std::fstream rs(replicasetfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        rs << hostport << std::endl;

        boost::filesystem::path identityfile(directory);
        identityfile /= IdentityFilename;
        std::fstream ids(identityfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        ids << identity.id << std::endl;
    }
}


int main(int argc, char** argv)
{
    //
    // Parse the command line options.
    //
    std::string address;
    boost::uuids::uuid identity;

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("node-address",
         boost::program_options::value(&address)->default_value("127.0.0.1"),
         "address of the node")
        ("identity",
         boost::program_options::value(&identity),
         "uuid identity of the node")
    ;
    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        std::exit(EXIT_FAILURE);
    }

    //
    // Setup the partitions
    //
    if (vm.count("identity"))
    {
        SetupPartition(
            "p-minus",
            address + ":8070",
            dafs::Identity(boost::uuids::to_string(identity)) - dafs::Identity("00000000-0000-0000-0000-0000000000ff")
        );
        SetupPartition(
            "p-zero",
            address + ":8080",
            dafs::Identity(boost::uuids::to_string(identity))
        );
        SetupPartition(
            "p-plus",
            address + ":8090",
            dafs::Identity(boost::uuids::to_string(identity)) + dafs::Identity("00000000-0000-0000-0000-0000000000ff")
        );
    }

    //
    // Start node and server.
    //
    dafs::Node n(
        dafs::Address(address, 8070),
        dafs::Address(address, 8080),
        dafs::Address(address, 8090));
    dafs::Dispatcher dispatcher(n);

    auto server = boost::make_shared<dafs::Server>(address, 9000, dispatcher);
    server->Start();

    for (;;);
}
