#include "dafs/customhash.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"
#include "dafs/replication.hpp"


namespace dafs
{
    PaxosReplication::PaxosReplication(
        dafs::Address address,
        dafs::Root root)
    : in_progress(std::make_shared<dafs::Signal>()),
      parliament(Replica(address.ip, address.port),
                 root.directory,
                 dafs::Commit(root, in_progress))
    {
    }


    PaxosReplication::PaxosReplication(
        const PaxosReplication& other
    )
        : parliament(other.parliament),
          in_progress(std::make_shared<dafs::Signal>())
    {
    }


    void
    PaxosReplication::Write(std::string entry)
    {
        parliament.SendProposal(entry);
        in_progress->Wait();
    }


    void
    PaxosReplication::AddReplica(dafs::Address address, std::string location)
    {
        parliament.AddLegislator(address.ip, address.port, location);
    }


    void
    PaxosReplication::RemoveReplica(dafs::Address address)
    {
        parliament.RemoveLegislator(address.ip, address.port);
    }


    std::vector<dafs::Address>
    PaxosReplication::GetMissingReplicas()
    {
        auto nonresponsive = parliament.GetLegislators();
        for (auto i : parliament.GetAbsenteeBallots(10))
        {
            nonresponsive = nonresponsive->Intersection(i.second);
        }
        std::vector<dafs::Address> nonresponsive_endpoints;
        for (auto r : *nonresponsive)
        {
            nonresponsive_endpoints.push_back(dafs::Address(r.hostname, r.port));
        }
        return nonresponsive_endpoints;
    }
}
