#include "dafs/customhash.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"
#include "dafs/replication.hpp"
#include "dafs/result.hpp"


namespace dafs
{
    PaxosReplication::PaxosReplication(
        dafs::Address address,
        dafs::Root root)
    : progress_map(),
      parliament(Replica(address.ip, address.port),
                 root.directory,
                 dafs::Commit(root, progress_map),
                 dafs::Ignore(progress_map)),
      mutex()
    {
    }


    PaxosReplication::PaxosReplication(
        const PaxosReplication& other
    )
        : progress_map(other.progress_map),
          parliament(other.parliament),
          mutex()
    {
    }


    dafs::Result
    PaxosReplication::Write(std::string entry)
    {
        // Here mutex is to guard against concurrent write access to
        // progress_map.
        std::lock_guard<std::mutex> guard(mutex);

        auto proposal = dafs::Deserialize<dafs::Proposal>(entry);

        do
        {
            proposal.uuid = boost::uuids::random_generator()();
        }
        while (progress_map.find(proposal.uuid) != progress_map.end());

        progress_map[proposal.uuid] = std::make_shared<dafs::Signal>();

        parliament.SendProposal(dafs::Serialize(proposal));

        auto result = progress_map[proposal.uuid]->Wait();
        progress_map.erase(proposal.uuid);
        return result;
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
        int minimum_ballot_count = 10;
        auto absentee_ballots = parliament.GetAbsenteeBallots(minimum_ballot_count);

        if (absentee_ballots.size() < minimum_ballot_count)
        {
            //
            // We don't have enough data to suggest missing replica.
            //
            return std::vector<dafs::Address>{};
        }

        auto nonresponsive = parliament.GetLegislators();

        for (auto i : parliament.GetAbsenteeBallots(minimum_ballot_count))
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
