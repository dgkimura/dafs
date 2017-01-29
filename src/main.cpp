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
ParseOptions(int argc, char** argv)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("address", boost::program_options::value<std::string>(), "address of the node")
        ("identity", boost::program_options::value<boost::uuids::uuid>(), "uuid identity of the node")
    ;
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        std::exit(EXIT_FAILURE);
    }
}


void
SetupPartition(std::string directory, std::string hostport)
{
    if (!boost::filesystem::exists(directory))
    {
        boost::filesystem::create_directory(directory);
        boost::filesystem::path replicasetfile(directory);
        replicasetfile /= ReplicasetFilename;
        std::fstream s(replicasetfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        s << hostport << std::endl;
    }
}


int main(int argc, char** argv)
{
    ParseOptions(argc, argv);

    SetupPartition("p-minus", "127.0.0.1:8070");
    SetupPartition("p-zero", "127.0.0.1:8080");
    SetupPartition("p-plus", "127.0.0.1:8090");

    dafs::Node n;

    n.GetPartition(dafs::Node::Slot::Minus).SetIdentity(dafs::Identity{"00000000-0000-0000-0000-000000000000"});
    n.GetPartition(dafs::Node::Slot::Minus).SetIdentity(dafs::Identity{"11111111-1111-1111-1111-111111111111"});
    n.GetPartition(dafs::Node::Slot::Zero).SetIdentity(dafs::Identity{"22222222-2222-2222-2222-222222222222"});
    n.GetPartition(dafs::Node::Slot::Plus).SetIdentity(dafs::Identity{"33333333-3333-3333-3333-333333333333"});
    n.GetPartition(dafs::Node::Slot::Plus).SetIdentity(dafs::Identity{"44444444-4444-4444-4444-444444444444"});

    n.GetPartition(dafs::Node::Slot::Plus).CreateBlock(dafs::BlockInfo{"myblock", dafs::Identity("01234567-89ab-cdef-0123-456789abcdef"), 0});
    n.GetPartition(dafs::Node::Slot::Plus).CreateBlock(dafs::BlockInfo{"yourblock", dafs::Identity(), 0});
    n.GetPartition(dafs::Node::Slot::Plus).CreateBlock(dafs::BlockInfo{"ourblock", dafs::Identity(), 0});
    n.GetPartition(dafs::Node::Slot::Plus).WriteBlock(dafs::BlockInfo{"myblock", dafs::Identity("01234567-89ab-cdef-0123-456789abcdef"), 0},
                                                      dafs::BlockFormat{"contents of myblock"});

    n.GetPartition(dafs::Node::Slot::Plus).AddNode("1.1.1.1", 1111);
    n.GetPartition(dafs::Node::Slot::Zero).AddNode("2.2.2.2", 2222);
    n.GetPartition(dafs::Node::Slot::Zero).RemoveNode("2.2.2.2", 2222);

    dafs::NetworkSender sender;

    dafs::Dispatcher dispatcher(n, sender);

    dafs::Server server("127.0.0.1111111", 8000, dispatcher);

    for (;;);
}
