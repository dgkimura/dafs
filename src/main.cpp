#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid/uuid.hpp>

#include "dafs/dispatcher.hpp"
#include "dafs/main.hpp"
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
        ("address",
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
    dafs::Node n;

    n.GetPartition(dafs::Node::Slot::Minus)->WriteBlock(dafs::BlockInfo{"myblock", dafs::Identity("11111111-2222-3333-4444-555555555555"), 0},
                                                      dafs::BlockFormat{"first contents of myblock"});
    n.GetPartition(dafs::Node::Slot::Minus)->WriteBlock(dafs::BlockInfo{"myblock", dafs::Identity("11111111-2222-3333-4444-555555555555"), 0},
                                                      dafs::BlockFormat{"next contents of myblock"});

    n.GetPartition(dafs::Node::Slot::Minus)->SetIdentity(dafs::Identity{"00000000-0000-0000-0000-000000000000"});
    n.GetPartition(dafs::Node::Slot::Minus)->SetIdentity(dafs::Identity{"11111111-1111-1111-1111-111111111111"});
    n.GetPartition(dafs::Node::Slot::Zero)->SetIdentity(dafs::Identity{"22222222-2222-2222-2222-222222222222"});
    n.GetPartition(dafs::Node::Slot::Plus)->SetIdentity(dafs::Identity{"33333333-3333-3333-3333-333333333333"});
    n.GetPartition(dafs::Node::Slot::Plus)->SetIdentity(dafs::Identity{"44444444-4444-4444-4444-444444444444"});

    n.GetPartition(dafs::Node::Slot::Plus)->CreateBlock(dafs::BlockInfo{"myblock", dafs::Identity("01234567-89ab-cdef-0123-456789abcdef"), 0});
    n.GetPartition(dafs::Node::Slot::Plus)->CreateBlock(dafs::BlockInfo{"yourblock", dafs::Identity(), 0});
    n.GetPartition(dafs::Node::Slot::Plus)->CreateBlock(dafs::BlockInfo{"ourblock", dafs::Identity(), 0});
    n.GetPartition(dafs::Node::Slot::Plus)->WriteBlock(dafs::BlockInfo{"myblock", dafs::Identity("01234567-89ab-cdef-0123-456789abcdef"), 0},
                                                      dafs::BlockFormat{"contents of myblock"});

    n.GetPartition(dafs::Node::Slot::Plus)->AddNode("1.1.1.1", 1111);
    n.GetPartition(dafs::Node::Slot::Zero)->AddNode("2.2.2.2", 2222);
    n.GetPartition(dafs::Node::Slot::Zero)->RemoveNode("2.2.2.2", 2222);

    dafs::NetworkSender sender;

    dafs::Dispatcher dispatcher(n, sender);

    dafs::Server server(address, 8000, dispatcher);

    for (;;);
}
