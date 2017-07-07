#include <gtest/gtest.h>

#include <boost/filesystem.hpp>
#include <dafs/partition.hpp>

#include "mocks.hpp"


TEST(PartitionTest, testReplicatedPartitionStartsPingComponent)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());

    EXPECT_CALL(*ping, Start()).Times(1);

    dafs::ReplicatedPartition partition(
        std::unique_ptr<MockReplication>(new MockReplication()),
        std::shared_ptr<MockStorage>(new MockStorage()),
        std::unique_ptr<MockNodeSet>(new MockNodeSet()),
        std::move(ping),
        std::shared_ptr<MockLock>(new MockLock())
    );
}


TEST(PartitionTest, testGetIdentityWillParseDetailsFile)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };
    auto format = dafs::BlockFormat {
        dafs::Serialize(
            dafs::ReplicatedEndpoints {
                dafs::Endpoint {},
                dafs::Endpoint {
                    dafs::Address{},
                    dafs::Address{},
                    dafs::Identity("22222222-2222-2222-2222-222222222222") },
                dafs::Endpoint {}
            }
        )
    };

    EXPECT_CALL(*ping, Start()).Times(1);

    dafs::ReplicatedPartition partition(
        std::unique_ptr<MockReplication>(new MockReplication()),
        storage,
        std::unique_ptr<MockNodeSet>(new MockNodeSet()),
        std::move(ping),
        std::shared_ptr<MockLock>(new MockLock())
    );

    EXPECT_CALL(*storage, ReadBlock(info))
        .Times(1)
        .WillOnce(testing::Return(format));

    auto identity = partition.GetIdentity();
    ASSERT_EQ("22222222-2222-2222-2222-222222222222", identity.id);
}
