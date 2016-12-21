#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "commit.hpp"
#include "customhash.hpp"
#include "nodeset.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedNodeSet::ReplicatedNodeSet(
        Parliament& parliament,
        dafs::Signal& in_progress)
    : parliament(parliament),
      in_progress(in_progress)
    {
    }


    ReplicatedNodeSet::ReplicatedNodeSet(
        const ReplicatedNodeSet& other)
    : parliament(other.parliament),
      in_progress(other.in_progress)
    {
    }


    void
    ReplicatedNodeSet::AddNode(std::string address, short port)
    {
        parliament.AddLegislator(address, port);
        in_progress.Wait();
    }


    void
    ReplicatedNodeSet::RemoveNode(std::string address, short port)
    {
        parliament.RemoveLegislator(address, port);
        in_progress.Wait();
    }
}
