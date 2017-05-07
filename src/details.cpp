#include "dafs/details.hpp"


namespace dafs
{
    dafs::Endpoint
    GetFailover(
        dafs::ReplicatedEndpoints endpoints,
        dafs::Address active,
        dafs::Address inactive)
    {
        if (endpoints.minus.replication.ip != active.ip &&
            endpoints.minus.replication.port != active.port &&
            endpoints.minus.replication.ip != inactive.ip &&
            endpoints.minus.replication.port != inactive.port)
        {
            return endpoints.minus;
        }
        if (endpoints.zero.replication.ip != active.ip &&
            endpoints.zero.replication.port != active.port &&
            endpoints.zero.replication.ip != inactive.ip &&
            endpoints.zero.replication.port != inactive.port)
        {
            return endpoints.zero;
        }
        if (endpoints.plus.replication.ip != active.ip &&
            endpoints.plus.replication.port != active.port &&
            endpoints.plus.replication.ip != inactive.ip &&
            endpoints.plus.replication.port != inactive.port)
        {
            return endpoints.plus;
        }
        return {
            dafs::EmptyAddress(),
            dafs::EmptyAddress()
        };
    }
}
