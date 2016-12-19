#include <boost/filesystem.hpp>

#include <paxos/parliament.hpp>

#include "commit.hpp"
#include "filesystem.hpp"
#include "main.hpp"
#include "partition.hpp"
#include "signal.hpp"

dafs::Signal in_progress;


int main(void)
{
    std::string directory = "node-1";
    std::string hostport = "127.0.0.1:8080";

    if (!boost::filesystem::exists(directory))
    {
        boost::filesystem::create_directory(directory);
        boost::filesystem::path replicasetfile(directory);
        replicasetfile /= "replicaset";
        std::fstream s(replicasetfile.string(), std::ios::in | std::ios::out | std::ios::trunc);
        s << "127.0.0.1:8080" << std::endl;
    }

    dafs::BlockInfo files_info = dafs::CreateBlockInfo(
        (fs::path(directory) / fs::path(Constant::FileListName)).string(),
        dafs::CreateLocation(hostport)
    );
    dafs::BlockInfo blocks_info = dafs::CreateBlockInfo(
        (fs::path(directory) / fs::path(Constant::BlockListName)).string(),
        dafs::CreateLocation(hostport)
    );
    dafs::BlockInfo nodeset_info = dafs::CreateBlockInfo(
        (fs::path(directory) / fs::path(Constant::NodeSetName)).string(),
        dafs::CreateLocation(hostport)
    );
    dafs::BlockInfo identity_info = dafs::CreateBlockInfo(
        (fs::path(directory) / fs::path(Constant::IdentityName)).string(),
        dafs::CreateLocation(hostport)
    );

    Parliament parliament(directory, DecreeHandler(dafs::Commit(parliament, in_progress)));
    //parliament.AddLegislator("127.0.0.1", 8080);

    auto store = dafs::ReplicatedStorage(parliament, in_progress);
    auto nodeset = dafs::ReplicatedNodeSet(parliament, in_progress);

    auto partition = dafs::Partition(
        store,
        nodeset,
        files_info,
        blocks_info,
        nodeset_info,
        identity_info
    );
    partition.SetIdentity(111);
    partition.SetIdentity(222);
    partition.SetIdentity(333);
    partition.SetIdentity(444);
    partition.SetIdentity(555);
    partition.AddNode("1.1.1.1", 1111);
    partition.AddNode("2.2.2.2", 2222);
    partition.AddNode("3.3.3.3", 3333);
    partition.AddNode("4.4.4.4", 4444);
    //partition.RemoveNode("2.2.2.2", 8081);

    for (;;);
}
