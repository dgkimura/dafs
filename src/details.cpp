#include <unordered_map>

#include "dafs/customhash.hpp"
#include "dafs/details.hpp"


namespace dafs
{
    dafs::Endpoint
    GetFailover(
        dafs::ReplicatedEndpoints endpoints,
        dafs::Address active,
        dafs::Address inactive)
    {
        std::unordered_map<dafs::Address, dafs::Endpoint> items;
        items[endpoints.minus.replication] = endpoints.minus;
        items[endpoints.zero.replication] = endpoints.zero;
        items[endpoints.plus.replication] = endpoints.plus;

        items.erase(active);
        items.erase(inactive);

        return items.size() == 1 ? items.begin()->second :
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::EmptyAddress()
            };
    }


    bool
    IsReplicatedPartition(
        dafs::ReplicatedEndpoints endpoints)
    {
        return endpoints.minus.replication.ip != dafs::EmptyAddress().ip &&
               endpoints.minus.replication.port != dafs::EmptyAddress().port &&
               endpoints.zero.replication.ip != dafs::EmptyAddress().ip &&
               endpoints.zero.replication.port != dafs::EmptyAddress().port &&
               endpoints.plus.replication.ip != dafs::EmptyAddress().ip &&
               endpoints.plus.replication.port != dafs::EmptyAddress().port;
    }
}
