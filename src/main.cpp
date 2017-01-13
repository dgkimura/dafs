#include <boost/filesystem.hpp>

#include "main.hpp"
#include "node.hpp"


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


int main(void)
{
    SetupPartition("p-minus", "127.0.0.1:8070");
    SetupPartition("p-zero", "127.0.0.1:8080");
    SetupPartition("p-plus", "127.0.0.1:8090");

    dafs::Node n;

    n.GetPartition(dafs::Node::Slot::Minus).SetIdentity(dafs::Partition::Identity{"111"});
    n.GetPartition(dafs::Node::Slot::Minus).SetIdentity(dafs::Partition::Identity{"222"});
    n.GetPartition(dafs::Node::Slot::Zero).SetIdentity(dafs::Partition::Identity{"333"});
    n.GetPartition(dafs::Node::Slot::Plus).SetIdentity(dafs::Partition::Identity{"444"});
    n.GetPartition(dafs::Node::Slot::Plus).SetIdentity(dafs::Partition::Identity{"555"});

    n.GetPartition(dafs::Node::Slot::Plus).CreateBlock(dafs::BlockInfo{"myblock", "1.1.1.1", 1111, 0});
    n.GetPartition(dafs::Node::Slot::Plus).CreateBlock(dafs::BlockInfo{"yourblock", "2.2.2.2", 2222, 0});
    n.GetPartition(dafs::Node::Slot::Plus).CreateBlock(dafs::BlockInfo{"ourblock", "3.3.3.3", 3333, 0});
    n.GetPartition(dafs::Node::Slot::Plus).WriteBlock(dafs::BlockInfo{"myblock", "1.1.1.1", 1111, 0},
                                                      dafs::BlockFormat{"contents of myblock"});

    n.GetPartition(dafs::Node::Slot::Plus).AddNode("1.1.1.1", 1111);
    n.GetPartition(dafs::Node::Slot::Zero).AddNode("2.2.2.2", 2222);
    n.GetPartition(dafs::Node::Slot::Zero).RemoveNode("2.2.2.2", 2222);

    for (;;);
}
