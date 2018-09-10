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


TEST_F(NodeTest, x)
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
                dafs::Endpoint
                {
                    dafs::Address("1.1", 11),
                    dafs::Address("1.1.1.1", 1111),
                    dafs::Identity("11111111-1111-1111-1111-111111111111"),
                    "fault-domain-1"
                },
                dafs::Endpoint
                {
                    dafs::Address("2.2", 22),
                    dafs::Address("2.2.2.2", 2222),
                    dafs::Identity("22222222-2222-2222-2222-222222222222"),
                    "fault-domain-2"
                },
                dafs::Endpoint
                {
                    dafs::Address("3.3", 33),
                    dafs::Address("3.3.3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333"),
                    "fault-domain-3"
                }
            }));
    EXPECT_CALL(*mock_zero_partition, GetNodeSetDetails())
        .Times(1)
        .WillRepeatedly(testing::Return(
            dafs::ReplicatedEndpoints
            {
                dafs::Endpoint
                {
                    dafs::Address("4.4", 44),
                    dafs::Address("4.4.4.4", 4444),
                    dafs::Identity("44444444-4444-4444-4444-444444444444"),
                    "fault-domain-4"
                },
                dafs::Endpoint
                {
                    dafs::Address("5.5", 55),
                    dafs::Address("5.5.5.5", 5555),
                    dafs::Identity("55555555-5555-5555-5555-555555555555"),
                    "fault-domain-5"
                },
                dafs::Endpoint
                {
                    dafs::Address("6.6", 66),
                    dafs::Address("6.6.6.6", 6666),
                    dafs::Identity("66666666-6666-6666-6666-666666666666"),
                    "fault-domain-6"
                }
            }));
    EXPECT_CALL(*mock_plus_partition, GetNodeSetDetails())
        .Times(1)
        .WillRepeatedly(testing::Return(
            dafs::ReplicatedEndpoints
            {
                dafs::Endpoint
                {
                    dafs::Address("7.7", 77),
                    dafs::Address("7.7.7.7", 7777),
                    dafs::Identity("77777777-7777-7777-7777-777777777777"),
                    "fault-domain-7"
                },
                dafs::Endpoint
                {
                    dafs::Address("8.8", 88),
                    dafs::Address("8.8.8.8", 8888),
                    dafs::Identity("88888888-8888-8888-8888-888888888888"),
                    "fault-domain-8"
                },
                dafs::Endpoint
                {
                    dafs::Address("9.9", 99),
                    dafs::Address("9.9.9.9", 9999),
                    dafs::Identity("99999999-9999-9999-9999-999999999999"),
                    "fault-domain-9"
                }
            }));

    ASSERT_EQ(0, GetFaultDomainViolationEndpoints(node).size());
}
