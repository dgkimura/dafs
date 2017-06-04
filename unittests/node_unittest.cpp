#include <memory>

#include <gtest/gtest.h>

#include "dafs/node.hpp"
#include "mock_partition.hpp"


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

        node = std::unique_ptr<dafs::Node>(
            new dafs::Node(
                partition_minus, partition_zero, partition_plus
            )
        );

    }

public:

    std::unique_ptr<dafs::Node> node;
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
