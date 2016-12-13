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

    Parliament parliament(directory, dafs::Commit(parliament, in_progress));
    parliament.AddLegislator("127.0.0.1", 8080);

    auto store = dafs::ReplicatedStorage(parliament, in_progress);
    auto nodeset = dafs::ReplicatedNodeSet(parliament, nodeset_info, in_progress);

    auto partition = dafs::Partition(
        store,
        nodeset,
        files_info,
        blocks_info,
        nodeset_info,
        identity_info
    );
    partition.AddNode("2.2.2.2", 8081);
    partition.AddNode("1.1.1.1", 1111);
    partition.RemoveNode("2.2.2.2", 8081);

    for (;;);
}
