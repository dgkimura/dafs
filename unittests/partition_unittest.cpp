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


TEST(PartitionTest, testGetNodeSetDetailsWillParseDetailsFile)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };
    auto format = dafs::BlockFormat {
        dafs::Serialize(
            dafs::ReplicatedEndpoints {
                dafs::Endpoint {
                    dafs::Address{"1.0.0.0", 100},
                    dafs::Address{"1.0.0.1", 101},
                    dafs::Identity("11111111-1111-1111-1111-111111111111") },
                dafs::Endpoint {
                    dafs::Address{"2.0.0.0", 200},
                    dafs::Address{"2.0.0.1", 201},
                    dafs::Identity("22222222-2222-2222-2222-222222222222") },
                dafs::Endpoint {
                    dafs::Address{"3.0.0.0", 300},
                    dafs::Address{"3.0.0.1", 301},
                    dafs::Identity("33333333-3333-3333-3333-333333333333") },
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

    auto details = partition.GetNodeSetDetails();

    ASSERT_EQ("11111111-1111-1111-1111-111111111111", details.minus.identity.id);
    ASSERT_EQ("1.0.0.0", details.minus.management.ip);
    ASSERT_EQ(100, details.minus.management.port);
    ASSERT_EQ("1.0.0.1", details.minus.replication.ip);
    ASSERT_EQ(101, details.minus.replication.port);
    ASSERT_EQ("22222222-2222-2222-2222-222222222222", details.zero.identity.id);
    ASSERT_EQ("2.0.0.0", details.zero.management.ip);
    ASSERT_EQ(200, details.zero.management.port);
    ASSERT_EQ("2.0.0.1", details.zero.replication.ip);
    ASSERT_EQ(201, details.zero.replication.port);
    ASSERT_EQ("33333333-3333-3333-3333-333333333333", details.plus.identity.id);
    ASSERT_EQ("3.0.0.0", details.plus.management.ip);
    ASSERT_EQ(300, details.plus.management.port);
    ASSERT_EQ("3.0.0.1", details.plus.replication.ip);
    ASSERT_EQ(301, details.plus.replication.port);
}


TEST(PartitionTest, testDeleteBlockDeletesBlockAndRemovesIndex)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };

    EXPECT_CALL(*ping, Start()).Times(1);

    dafs::ReplicatedPartition partition(
        std::unique_ptr<MockReplication>(new MockReplication()),
        storage,
        std::unique_ptr<MockNodeSet>(new MockNodeSet()),
        std::move(ping),
        std::shared_ptr<MockLock>(new MockLock())
    );

    EXPECT_CALL(*storage, DeleteBlock(info))
        .Times(1);
    EXPECT_CALL(*storage, RemoveIndex(info))
        .Times(1);

    partition.DeleteBlock(info);
}


TEST(PartitionTest, testReadBlockParsesBlockFormat)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };
    auto format = dafs::BlockFormat { "The contents of a block format." };

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

    ASSERT_EQ(format.contents, partition.ReadBlock(info).contents);
}


TEST(PartitionTest, testGetIndexReturnsBlockIndex)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };

    EXPECT_CALL(*ping, Start()).Times(1);

    dafs::ReplicatedPartition partition(
        std::unique_ptr<MockReplication>(new MockReplication()),
        storage,
        std::unique_ptr<MockNodeSet>(new MockNodeSet()),
        std::move(ping),
        std::shared_ptr<MockLock>(new MockLock())
    );

    EXPECT_CALL(*storage, GetIndex())
        .Times(1)
        .WillOnce(testing::Return(
            dafs::BlockIndex
            {
                std::vector<dafs::BlockInfo>{info}
            }));

    auto index = partition.GetIndex();
    ASSERT_EQ(info.path, index.items[0].path);
    ASSERT_EQ(info.identity, index.items[0].identity);
    ASSERT_EQ(info.revision, index.items[0].revision);
}


TEST(PartitionTest, testIsActiveWithNonemptyAddresses)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };
    auto format = dafs::BlockFormat {
        dafs::Serialize(
            dafs::ReplicatedEndpoints {
                dafs::Endpoint {
                    dafs::Address{"1.0.0.0", 100},
                    dafs::Address{"1.0.0.1", 101},
                    dafs::Identity("11111111-1111-1111-1111-111111111111") },
                dafs::Endpoint {
                    dafs::Address{"2.0.0.0", 200},
                    dafs::Address{"2.0.0.1", 201},
                    dafs::Identity("22222222-2222-2222-2222-222222222222") },
                dafs::Endpoint {
                    dafs::Address{"3.0.0.0", 300},
                    dafs::Address{"3.0.0.1", 301},
                    dafs::Identity("33333333-3333-3333-3333-333333333333") },
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

    ASSERT_TRUE(partition.IsActive());
}


TEST(PartitionTest, testIsActiveWithEmptyAddress)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto info = dafs::BlockInfo { Constant::DetailsName, dafs::Identity() };

    // format details contains a dafs::EmptyAddress
    auto format = dafs::BlockFormat {
        dafs::Serialize(
            dafs::ReplicatedEndpoints {
                dafs::Endpoint {
                    dafs::Address{"1.0.0.0", 100},
                    dafs::Address{"1.0.0.1", 101},
                    dafs::Identity("11111111-1111-1111-1111-111111111111") },
                dafs::Endpoint {
                    dafs::Address{"2.0.0.0", 200},
                    dafs::Address{"2.0.0.1", 201},
                    dafs::Identity("22222222-2222-2222-2222-222222222222") },
                dafs::Endpoint {
                    dafs::Address{"3.0.0.0", 300},
                    dafs::EmptyAddress(),
                    dafs::Identity("33333333-3333-3333-3333-333333333333") },
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

    ASSERT_FALSE(partition.IsActive());
}


TEST(PartitionTest, testPartitionLockAcquire)
{
    auto ping = std::unique_ptr<MockPing>(new MockPing());
    auto lock = std::shared_ptr<MockLock>(new MockLock());

    EXPECT_CALL(*ping, Start()).Times(1);

    dafs::ReplicatedPartition partition(
        std::unique_ptr<MockReplication>(new MockReplication()),
        std::shared_ptr<MockStorage>(new MockStorage()),
        std::unique_ptr<MockNodeSet>(new MockNodeSet()),
        std::move(ping),
        lock
    );

    EXPECT_CALL(*lock, Acquire())
        .Times(1)
        .WillOnce(testing::Return(true));
    ASSERT_TRUE(partition.Acquire());

    EXPECT_CALL(*lock, Acquire())
        .Times(1)
        .WillOnce(testing::Return(false));
    ASSERT_FALSE(partition.Acquire());
}
