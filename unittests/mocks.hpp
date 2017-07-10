#include <gmock/gmock.h>

#include <dafs/replication.hpp>
#include <dafs/partition.hpp>


class MockReplication : public dafs::Replication
{
public:

    MOCK_METHOD1(Write, dafs::Result(std::string entry));

    MOCK_METHOD2(AddReplica, void(dafs::Address address, std::string location));

    MOCK_METHOD1(RemoveReplica, void(dafs::Address address));

    MOCK_METHOD0(GetMissingReplicas, std::vector<dafs::Address>());
};

class MockStorage : public dafs::Storage
{
public:

    MOCK_METHOD1(ReadBlock, dafs::BlockFormat(dafs::BlockInfo block));

    MOCK_METHOD1(DeleteBlock, void(dafs::BlockInfo info));

    MOCK_METHOD1(InsertIndex, void(dafs::BlockInfo info));

    MOCK_METHOD1(RemoveIndex, void(dafs::BlockInfo info));

    MOCK_METHOD0(GetIndex, dafs::BlockIndex());

    MOCK_METHOD2(WriteBlock, void(dafs::BlockInfo info, dafs::BlockFormat data));

    MOCK_METHOD2(Write, void(dafs::BlockInfo info, dafs::Delta delta));
};


class MockNodeSet : public dafs::NodeSet
{
public:

    MOCK_METHOD1(RemoveNode, void(dafs::Address address));

    MOCK_METHOD5(SetMinus, dafs::ReplicatedEndpoints(
        const dafs::Address management,
        const dafs::Address replication,
        const dafs::Identity identity,
        const std::string location,
        dafs::ReplicatedEndpoints details));

    MOCK_METHOD5(SetZero, dafs::ReplicatedEndpoints(
        const dafs::Address management,
        const dafs::Address replication,
        const dafs::Identity identity,
        const std::string location,
        dafs::ReplicatedEndpoints details));

    MOCK_METHOD5(SetPlus, dafs::ReplicatedEndpoints(
        const dafs::Address management,
        const dafs::Address replication,
        const dafs::Identity identity,
        const std::string location,
        dafs::ReplicatedEndpoints details));
};


class MockPing : public dafs::Ping
{
public:

    MOCK_METHOD1(NonresponsiveMembers, std::vector<dafs::Address>(int last_elections));

    MOCK_METHOD0(Start, void());

    MOCK_METHOD1(SendPing, void(std::shared_ptr<dafs::Sender> sender));
};


class MockLock : public dafs::Lock
{
public:

    MOCK_METHOD0(Acquire, bool());

    MOCK_METHOD0(IsLocked, bool());

    MOCK_METHOD0(Release, void());
};