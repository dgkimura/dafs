#include <boost/filesystem.hpp>

#include "main.hpp"
#include "partition.hpp"


int main(void)
{
    std::string directory = "node-1";

    if (!boost::filesystem::exists(directory))
    {
        boost::filesystem::create_directory(directory);
        boost::filesystem::path replicasetfile(directory);
        replicasetfile /= "replicaset";
        std::fstream s(replicasetfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        s << "127.0.0.1:8080" << std::endl;
    }

    auto partition = dafs::Partition(
        dafs::Root(directory)
    );
    partition.SetIdentity(dafs::Partition::Identity{111});
    partition.SetIdentity(dafs::Partition::Identity{222});
    partition.SetIdentity(dafs::Partition::Identity{333});
    partition.SetIdentity(dafs::Partition::Identity{444});
    partition.SetIdentity(dafs::Partition::Identity{555});
    partition.AddNode("1.1.1.1", 1111);
    partition.AddNode("2.2.2.2", 2222);
    partition.AddNode("3.3.3.3", 3333);
    partition.AddNode("4.4.4.4", 4444);
    //partition.RemoveNode("2.2.2.2", 8081);

    for (;;);
}
