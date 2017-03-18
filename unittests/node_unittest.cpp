#include "gtest/gtest.h"

#include "dafs/node.hpp"
#include "mock_partition.hpp"


TEST(NodeTest, testGetPartition)
{
    auto partition_minus = std::make_shared<MockPartition>(
        dafs::Address("1.1.1.1", 1000),
        dafs::Address("1.1.1.1", 1001),
        dafs::Identity("11111111-1111-1111-1111-111111111111")
    );
    auto partition_zero = std::make_shared<MockPartition>(
        dafs::Address("2.2.2.2", 1000),
        dafs::Address("2.2.2.2", 1001),
        dafs::Identity("22222222-2222-2222-2222-222222222222")
    );
    auto partition_plus = std::make_shared<MockPartition>(
        dafs::Address("3.3.3.3", 1000),
        dafs::Address("3.3.3.3", 1001),
        dafs::Identity("33333333-3333-3333-3333-333333333333")
    );

    dafs::Node node(partition_minus, partition_zero, partition_plus);

    ASSERT_EQ(
        partition_minus->GetDetails().identity,
        node.GetPartition(dafs::Node::Slot::Minus)->GetDetails().identity);
    ASSERT_EQ(
        partition_zero->GetDetails().identity,
        node.GetPartition(dafs::Node::Slot::Zero)->GetDetails().identity);
    ASSERT_EQ(
        partition_plus->GetDetails().identity,
        node.GetPartition(dafs::Node::Slot::Plus)->GetDetails().identity);
}
