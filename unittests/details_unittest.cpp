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
            dafs::Address("A_Replication", 11)
        },
        dafs::Endpoint
        {
            dafs::Address("B_Management", 2),
            dafs::Address("B_Replication", 22)
        },
        dafs::Endpoint
        {
            dafs::Address("C_Management", 3),
            dafs::Address("C_Replication", 33)
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
