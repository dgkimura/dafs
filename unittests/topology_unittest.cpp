#include "gtest/gtest.h"

#include "topology.hpp"


TEST(TopologyTest, testGetPrimaryWithValidTopology)
{
    dafs::Topology t
    {
        {
            dafs::Node
            {
                1,
                "1.1.1.1"
            },
            dafs::Node
            {
                2,
                "2.2.2.2"
            },
            dafs::Node
            {
                3,
                "3.3.3.3"
            },
            dafs::Node
            {
                4,
                "4.4.4.4"
            },
            dafs::Node
            {
                5,
                "5.5.5.5"
            },
            dafs::Node
            {
                6,
                "6.6.6.6"
            },
            dafs::Node
            {
                7,
                "7.7.7.7"
            }
        }
    };

    dafs::Node primary = GetPrimary(t);

    ASSERT_EQ(primary.id, 4);
    ASSERT_EQ(primary.address, "4.4.4.4");
}


TEST(TopologyTest, testGetLeftWithValidTopology)
{
    dafs::Topology t
    {
        {
            dafs::Node
            {
                1,
                "1.1.1.1"
            },
            dafs::Node
            {
                2,
                "2.2.2.2"
            },
            dafs::Node
            {
                3,
                "3.3.3.3"
            },
            dafs::Node
            {
                4,
                "4.4.4.4"
            },
            dafs::Node
            {
                5,
                "5.5.5.5"
            },
            dafs::Node
            {
                6,
                "6.6.6.6"
            },
            dafs::Node
            {
                7,
                "7.7.7.7"
            }
        }
    };

    ASSERT_EQ(GetLeft(t, 1).id, 3);
    ASSERT_EQ(GetLeft(t, 2).id, 2);
    ASSERT_EQ(GetLeft(t, 3).id, 1);
}

TEST(TopologyTest, testGetRightWithValidTopology)
{
    dafs::Topology t
    {
        {
            dafs::Node
            {
                1,
                "1.1.1.1"
            },
            dafs::Node
            {
                2,
                "2.2.2.2"
            },
            dafs::Node
            {
                3,
                "3.3.3.3"
            },
            dafs::Node
            {
                4,
                "4.4.4.4"
            },
            dafs::Node
            {
                5,
                "5.5.5.5"
            },
            dafs::Node
            {
                6,
                "6.6.6.6"
            },
            dafs::Node
            {
                7,
                "7.7.7.7"
            }
        }
    };

    ASSERT_EQ(GetRight(t, 1).id, 5);
    ASSERT_EQ(GetRight(t, 2).id, 6);
    ASSERT_EQ(GetRight(t, 3).id, 7);
}
