#include <cstring>

#include <gtest/gtest.h>

#include "dafs/details.hpp"


TEST(DetailsTest, testGetFailoverWithValidCombinations)
{
    dafs::ReplicatedEndpoints endpoints
    {
        dafs::Endpoint
        {
            dafs::Address("A_Management", 1),
            dafs::Address("A_Replication", 11),
            dafs::Identity("11111111-1111-1111-1111-111111111111")
        },
        dafs::Endpoint
        {
            dafs::Address("B_Management", 2),
            dafs::Address("B_Replication", 22),
            dafs::Identity("22222222-2222-2222-2222-222222222222")
        },
        dafs::Endpoint
        {
            dafs::Address("C_Management", 3),
            dafs::Address("C_Replication", 33),
            dafs::Identity("33333333-3333-3333-3333-333333333333")
        },
    };

    ASSERT_EQ(
        "C_Replication",
        GetFailover(
            endpoints,
            dafs::Address("A_Replication", 11),
            dafs::Address("B_Replication", 22)).replication.ip
    );
    ASSERT_EQ(
        "C_Replication",
        GetFailover(
            endpoints,
            dafs::Address("B_Replication", 22),
            dafs::Address("A_Replication", 11)).replication.ip
    );
    ASSERT_EQ(
        "B_Replication",
        GetFailover(
            endpoints,
            dafs::Address("A_Replication", 11),
            dafs::Address("C_Replication", 33)).replication.ip
    );
    ASSERT_EQ(
        "B_Replication",
        GetFailover(
            endpoints,
            dafs::Address("C_Replication", 33),
            dafs::Address("A_Replication", 11)).replication.ip
    );
    ASSERT_EQ(
        "A_Replication",
        GetFailover(
            endpoints,
            dafs::Address("B_Replication", 22),
            dafs::Address("C_Replication", 33)).replication.ip
    );
    ASSERT_EQ(
        "A_Replication",
        GetFailover(
            endpoints,
            dafs::Address("C_Replication", 33),
            dafs::Address("B_Replication", 22)).replication.ip
    );
}


TEST(DetailsTest, testGetFailoverWithUnknownAddress)
{
    dafs::ReplicatedEndpoints endpoints
    {
        dafs::Endpoint
        {
            dafs::Address("A_Management", 1),
            dafs::Address("A_Replication", 11),
            dafs::Identity("11111111-1111-1111-1111-111111111111")
        },
        dafs::Endpoint
        {
            dafs::Address("B_Management", 2),
            dafs::Address("B_Replication", 22),
            dafs::Identity("22222222-2222-2222-2222-222222222222")
        },
        dafs::Endpoint
        {
            dafs::Address("C_Management", 3),
            dafs::Address("C_Replication", 33),
            dafs::Identity("33333333-3333-3333-3333-333333333333")
        },
    };

    ASSERT_EQ(
        "invalid_ip",
        GetFailover(
            endpoints,
            dafs::Address("UNKNOWN", 404),
            dafs::Address("B_Replication", 22)).replication.ip
    );
}


TEST(DetailsTest, testIsReplicatedPartitionWithoutEmptyAddress)
{
    dafs::ReplicatedEndpoints endpoints
    {
        dafs::Endpoint
        {
            dafs::Address("A_Management", 1),
            dafs::Address("A_Replication", 11),
            dafs::Identity("11111111-1111-1111-1111-111111111111")
        },
        dafs::Endpoint
        {
            dafs::Address("B_Management", 2),
            dafs::Address("B_Replication", 22),
            dafs::Identity("22222222-2222-2222-2222-222222222222")

        },
        dafs::Endpoint
        {
            dafs::Address("C_Management", 3),
            dafs::Address("C_Replication", 33),
            dafs::Identity("33333333-3333-3333-3333-333333333333")
        },
    };

    ASSERT_TRUE(IsReplicatedPartition(endpoints));
}


TEST(DetailsTest, testIsReplicatedPartitionWithEmptyAddress)
{
    dafs::ReplicatedEndpoints endpoints
    {
        dafs::Endpoint
        {
            dafs::Address("A_Management", 1),
            dafs::Address("A_Replication", 11),
            dafs::Identity("11111111-1111-1111-1111-111111111111")
        },
        dafs::Endpoint
        {
            dafs::Address("B_Management", 2),
            dafs::Address("B_Replication", 22),
            dafs::Identity("22222222-2222-2222-2222-222222222222")
        },
        dafs::Endpoint
        {
            dafs::Address("C_Management", 3),
            dafs::Address("C_Replication", 33),
            dafs::Identity("33333333-3333-3333-3333-333333333333")
        },
    };

    ASSERT_TRUE(IsReplicatedPartition(endpoints));

    dafs::ReplicatedEndpoints nonreplicated = endpoints;
    nonreplicated.minus.replication = dafs::EmptyAddress();

    ASSERT_FALSE(IsReplicatedPartition(nonreplicated));

    nonreplicated = endpoints;
    nonreplicated.zero.replication = dafs::EmptyAddress();

    ASSERT_FALSE(IsReplicatedPartition(nonreplicated));

    nonreplicated = endpoints;
    nonreplicated.plus.replication = dafs::EmptyAddress();

    ASSERT_FALSE(IsReplicatedPartition(nonreplicated));
}
