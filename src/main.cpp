#include <boost/filesystem.hpp>

#include <paxos/replicaset.hpp>

#include "main.hpp"
#include "partition.hpp"


dafs::Partition
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
    return dafs::Partition(dafs::Root(directory));
}


int main(void)
{
    auto partition_minus = SetupPartition("p-minus", "127.0.0.1:8070");
    auto partition_zero = SetupPartition("p-zero", "127.0.0.1:8080");
    auto partition_plus = SetupPartition("p-plus", "127.0.0.1:8090");

    partition_minus.SetIdentity(dafs::Partition::Identity{"111"});
    partition_minus.SetIdentity(dafs::Partition::Identity{"222"});
    partition_zero.SetIdentity(dafs::Partition::Identity{"333"});
    partition_plus.SetIdentity(dafs::Partition::Identity{"444"});
    partition_plus.SetIdentity(dafs::Partition::Identity{"555"});

    //partition_plus.AddNode("1.1.1.1", 1111);
    //partition_zero.AddNode("2.2.2.2", 2222);
    //partition_zero.AddNode("3.3.3.3", 3333);
    //partition_zero.AddNode("4.4.4.4", 4444);
    //partition.RemoveNode("2.2.2.2", 8081);

    partition_plus.CreateBlock(dafs::BlockInfo{"myblock", "1.1.1.1", 1111, 0});
    partition_plus.CreateBlock(dafs::BlockInfo{"yourblock", "2.2.2.2", 2222, 0});
    partition_plus.CreateBlock(dafs::BlockInfo{"ourblock", "3.3.3.3", 3333, 0});
    partition_plus.WriteBlock(dafs::BlockInfo{"myblock", "1.1.1.1", 1111, 0},
                              dafs::BlockFormat{"contents of myblock"});
    //partition_plus.DeleteBlock(dafs::BlockInfo{"myblock", "127.0.0.1", 8080, 0});

    for (;;);
}
