#include "dafs/customhash.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"
#include "dafs/replication.hpp"


namespace dafs
{
    PaxosReplication::PaxosReplication(
        dafs::Address address,
        dafs::Root root)
    : progress_map(),
      parliament(Replica(address.ip, address.port),
                 root.directory,
                 dafs::Commit(root, progress_map),
                 dafs::Ignore(progress_map))
    {
    }


    PaxosReplication::PaxosReplication(
        const PaxosReplication& other
    )
        : parliament(other.parliament),
          progress_map(other.progress_map)
    {
    }


    void
    PaxosReplication::Write(std::string entry)
    {
        auto proposal = dafs::Deserialize<dafs::Proposal>(entry);

        do
        {
            proposal.uuid = boost::uuids::random_generator()();
        } while (progress_map.find(proposal.uuid) != progress_map.end());

        progress_map[proposal.uuid] = std::make_shared<dafs::Signal>();

        parliament.SendProposal(dafs::Serialize(proposal));

        progress_map[proposal.uuid]->Wait();
        progress_map.erase(proposal.uuid);
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
