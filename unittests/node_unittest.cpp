#include <memory>

#include <gtest/gtest.h>

#include "dafs/node.hpp"
#include "mock_partition.hpp"
#include "mocks.hpp"


class NodeTest: public testing::Test
{
    virtual void SetUp()
    {
        auto partition_minus = std::make_shared<MockPartition>(
            dafs::Endpoint
            {
                dafs::Address("3.3.3.3", 3000),
                dafs::Address("3.3.3.3", 3333),
                dafs::Identity("33333333-3333-3333-3333-333333333333")

            },
            dafs::Endpoint
            {
                dafs::Address("1.1.1.1", 1000),
                dafs::Address("1.1.1.1", 1111),
                dafs::Identity("11111111-1111-1111-1111-111111111111")
            },
            dafs::Endpoint
            {
                dafs::Address("2.2.2.2", 2000),
                dafs::Address("2.2.2.2", 2222),
                dafs::Identity("22222222-2222-2222-2222-222222222222")
            }
        );
        auto partition_zero = std::make_shared<MockPartition>(
            dafs::Endpoint
            {
                dafs::Address("1.1.1.1", 1000),
                dafs::Address("1.1.1.1", 1111),
                dafs::Identity("11111111-1111-1111-1111-111111111111")
            },
            dafs::Endpoint
            {
                dafs::Address("2.2.2.2", 2000),
                dafs::Address("2.2.2.2", 2222),
                dafs::Identity("22222222-2222-2222-2222-222222222222")
            },
            dafs::Endpoint
            {
                dafs::Address("3.3.3.3", 3000),
                dafs::Address("3.3.3.3", 3333),
                dafs::Identity("33333333-3333-3333-3333-333333333333")
            }
        );
        auto partition_plus = std::make_shared<MockPartition>(
            dafs::Endpoint
            {
                dafs::Address("2.2.2.2", 2000),
                dafs::Address("2.2.2.2", 2222),
                dafs::Identity("22222222-2222-2222-2222-222222222222")
            },
            dafs::Endpoint
            {
                dafs::Address("3.3.3.3", 3000),
                dafs::Address("3.3.3.3", 3333),
                dafs::Identity("33333333-3333-3333-3333-333333333333")
            },
            dafs::Endpoint
            {
                dafs::Address("1.1.1.1", 1000),
                dafs::Address("1.1.1.1", 1111),
                dafs::Identity("11111111-1111-1111-1111-111111111111")
            }
        );

        node = std::unique_ptr<dafs::ReplicatedNode>(
            new dafs::ReplicatedNode(
                partition_minus, partition_zero, partition_plus
            )
        );

    }

public:

    std::unique_ptr<dafs::ReplicatedNode> node;
};


TEST_F(NodeTest, testGetPartitionWithSlotMinus)
{
    ASSERT_EQ(
        "11111111-1111-1111-1111-111111111111",
        node->GetPartition(dafs::Node::Slot::Minus)->GetIdentity().id);
    ASSERT_EQ(
        "22222222-2222-2222-2222-222222222222",
        node->GetPartition(dafs::Node::Slot::Zero)->GetIdentity().id);
    ASSERT_EQ(
        "33333333-3333-3333-3333-333333333333",
        node->GetPartition(dafs::Node::Slot::Plus)->GetIdentity().id);
}


TEST_F(NodeTest, testGetPartitionWithExactIdentity)
{
    ASSERT_EQ(
        "11111111-1111-1111-1111-111111111111",
        node->GetPartition(
            dafs::Identity("11111111-1111-1111-1111-111111111111")
        )->GetIdentity().id);
    ASSERT_EQ(
        "22222222-2222-2222-2222-222222222222",
        node->GetPartition(
            dafs::Identity("22222222-2222-2222-2222-222222222222")
        )->GetIdentity().id);
    ASSERT_EQ(
        "33333333-3333-3333-3333-333333333333",
        node->GetPartition(
            dafs::Identity("33333333-3333-3333-3333-333333333333")
        )->GetIdentity().id);
}


TEST_F(NodeTest, testGetPartitionWithApproximateIdentity)
{
    ASSERT_EQ(
        "11111111-1111-1111-1111-111111111111",
        node->GetPartition(
            dafs::Identity("11111111-2222-3333-4444-555555555555")
        )->GetIdentity().id);
    ASSERT_EQ(
        "22222222-2222-2222-2222-222222222222",
        node->GetPartition(
            dafs::Identity("22222222-3333-4444-5555-666666666666")
        )->GetIdentity().id);
    ASSERT_EQ(
        "33333333-3333-3333-3333-333333333333",
        node->GetPartition(
            dafs::Identity("33333333-4444-5555-6666-777777777777")
        )->GetIdentity().id);
}


TEST_F(NodeTest, testGetFaultDomainViolationEndpointsWithFiveUniqueFaultDomains)
{
    auto mock_minus_partition = std::make_shared<_MockPartition>();
    auto mock_zero_partition = std::make_shared<_MockPartition>();
    auto mock_plus_partition = std::make_shared<_MockPartition>();

    dafs::ReplicatedNode node(
        mock_minus_partition,
        mock_zero_partition,
        mock_plus_partition);

    EXPECT_CALL(*mock_minus_partition, GetNodeSetDetails())
        .Times(1)
        .WillRepeatedly(testing::Return(
            dafs::ReplicatedEndpoints
            {
                // Node 1
                dafs::Endpoint
                {
                    dafs::Address("1", 1),
                    dafs::Address("1.1.1.1", 1111),
                    dafs::Identity("11111111-1111-1111-1111-111111111111"),
                    "fault-domain-1"
                },
                // Node 2
                dafs::Endpoint
                {
                    dafs::Address("2", 2),
                    dafs::Address("2.2.2", 2222),
                    dafs::Identity("22222222-2222-2222-2222-222222222222"),
                    "fault-domain-2"
                },
                // Node 3
                dafs::Endpoint
                {
                    dafs::Address("3", 3),
                    dafs::Address("3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333"),
                    "fault-domain-3"
                }
            }));
    EXPECT_CALL(*mock_zero_partition, GetNodeSetDetails())
        .Times(1)
        .WillRepeatedly(testing::Return(
            dafs::ReplicatedEndpoints
            {
                // Node 2
                dafs::Endpoint
                {
                    dafs::Address("2", 2),
                    dafs::Address("2.2.2.2", 2222),
                    dafs::Identity("22222222-2222-2222-2222-222222222222"),
                    "fault-domain-2"
                },
                // Node 3
                dafs::Endpoint
                {
                    dafs::Address("3", 3),
                    dafs::Address("3.3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333"),
                    "fault-domain-3"
                },
                // Node 4
                dafs::Endpoint
                {
                    dafs::Address("4", 4),
                    dafs::Address("4.4", 4444),
                    dafs::Identity("44444444-4444-4444-4444-444444444444"),
                    "fault-domain-4"
                }
            }));
    EXPECT_CALL(*mock_plus_partition, GetNodeSetDetails())
        .Times(1)
        .WillRepeatedly(testing::Return(
            dafs::ReplicatedEndpoints
            {
                // Node 3
                dafs::Endpoint
                {
                    dafs::Address("3", 3),
                    dafs::Address("3.3.3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333"),
                    "fault-domain-3"
                },
                // Node 4
                dafs::Endpoint
                {
                    dafs::Address("4", 4),
                    dafs::Address("4.4.4", 4444),
                    dafs::Identity("44444444-4444-4444-4444-444444444444"),
                    "fault-domain-4"
                },
                // Node 5
                dafs::Endpoint
                {
                    dafs::Address("5", 5),
                    dafs::Address("5.5", 5555),
                    dafs::Identity("55555555-5555-5555-5555-555555555555"),
                    "fault-domain-5"
                }
            }));

    ASSERT_EQ(0, GetFaultDomainViolationEndpoints(node).size());
}
