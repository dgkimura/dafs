#include <vector>

#include <dafs/components.hpp>
#include <dafs/messages.hpp>
#include <gtest/gtest.h>

#include "mock_replication.hpp"
#include "mock_sender.hpp"


TEST(ReplicatedPingTest, testPingSendsProposeExitCluster)
{
    MockReplication replication(
        std::vector<dafs::Address>
        {
            dafs::Address("AWAY", 404)
        }
    );

    auto get_endpoints = [](){
        return dafs::ReplicatedEndpoints
        {
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::Address("A", 1)
            },
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::Address("AWAY", 404)
            },
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::Address("C", 3)
            }
        };
    };

    auto is_partition_locked = [](){ return false; };

    auto mock_sender = std::make_shared<MockSender>();
    dafs::ReplicatedPing ping(
        replication,
        dafs::Address("C", 3),
        get_endpoints,
        is_partition_locked,
        std::chrono::seconds(0));
    ping.SendPing(mock_sender);

    ASSERT_EQ(1, mock_sender->sentMessages().size());
    ASSERT_EQ(
        dafs::MessageType::_ProposeExitCluster,
        mock_sender->sentMessages()[0].type);
}


TEST(ReplicatedNodeSetTest, testSetMinusUpdatesReplicatedEndpointDetails)
{
    auto endpoints = dafs::ReplicatedEndpoints
    {
        dafs::Endpoint
        {
            dafs::Address{"1.0.0.0", 1000},
            dafs::Address{"1.0.0.1", 1001},
            dafs::Identity("10000000-0000-0000-000000000000")
        },
        dafs::Endpoint
        {
            dafs::Address{"2.0.0.0", 2000},
            dafs::Address{"2.0.0.1", 2001},
            dafs::Identity("20000000-0000-0000-000000000000")
        },
        dafs::Endpoint
        {
            dafs::Address{"3.0.0.0", 3000},
            dafs::Address{"3.0.0.1", 3001},
            dafs::Identity("30000000-0000-0000-000000000000")
        }
    };
    MockReplication replication;

    dafs::ReplicatedNodeSet nodeset(replication);
    auto result = nodeset.SetMinus(
        dafs::Address{"1.1.0.0", 1100},
        dafs::Address{"1.1.0.1", 1101},
        dafs::Identity("11111111-1111-1111-111111111111"),
        "a_location",
        endpoints
    );

    ASSERT_EQ("1.1.0.0", result.minus.management.ip);
    ASSERT_EQ(1100, result.minus.management.port);
    ASSERT_EQ("1.1.0.1", result.minus.replication.ip);
    ASSERT_EQ(1101, result.minus.replication.port);
    ASSERT_EQ("11111111-1111-1111-111111111111", result.minus.identity.id);
}


TEST(ReplicatedNodeSetTest, testSetPlusUpdatesReplicatedEndpointDetails)
{
    auto endpoints = dafs::ReplicatedEndpoints
    {
        dafs::Endpoint
        {
            dafs::Address{"1.0.0.0", 1000},
            dafs::Address{"1.0.0.1", 1001},
            dafs::Identity("10000000-0000-0000-000000000000")
        },
        dafs::Endpoint
        {
            dafs::Address{"2.0.0.0", 2000},
            dafs::Address{"2.0.0.1", 2001},
            dafs::Identity("20000000-0000-0000-000000000000")
        },
        dafs::Endpoint
        {
            dafs::Address{"3.0.0.0", 3000},
            dafs::Address{"3.0.0.1", 3001},
            dafs::Identity("30000000-0000-0000-000000000000")
        }
    };
    MockReplication replication;

    dafs::ReplicatedNodeSet nodeset(replication);
    auto result = nodeset.SetPlus(
        dafs::Address{"1.1.0.0", 1100},
        dafs::Address{"1.1.0.1", 1101},
        dafs::Identity("11111111-1111-1111-111111111111"),
        "a_location",
        endpoints
    );

    ASSERT_EQ("1.1.0.0", result.plus.management.ip);
    ASSERT_EQ(1100, result.plus.management.port);
    ASSERT_EQ("1.1.0.1", result.plus.replication.ip);
    ASSERT_EQ(1101, result.plus.replication.port);
    ASSERT_EQ("11111111-1111-1111-111111111111", result.plus.identity.id);
}
