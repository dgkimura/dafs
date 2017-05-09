#include <unordered_map>

#include <dafs/handler.hpp>
#include <dafs/node.hpp>
#include <gtest/gtest.h>

#include "mock_partition.hpp"
#include "mock_sender.hpp"


class HandlerTest: public testing::Test
{
    virtual void SetUp()
    {
        node_under_test = std::make_shared<dafs::Node>(
            std::make_shared<MockPartition>(
                dafs::Endpoint
                {
                    dafs::Address("1.1.1.1", 1000),
                    dafs::Address("1.1.1.1", 1111)
                },
                dafs::Endpoint
                {
                    dafs::Address("2.2.2.2", 2000),
                    dafs::Address("2.2.2.2", 2222)
                },
                dafs::Endpoint
                {
                    dafs::Address("3.3.3.3", 3000),
                    dafs::Address("3.3.3.3", 3333)
                },
                dafs::Identity("11111111-1111-1111-1111-111111111111")
            ),
            std::make_shared<MockPartition>(
                dafs::Endpoint
                {
                    dafs::Address("3.3.3.3", 3000),
                    dafs::Address("3.3.3.3", 3333)
                },
                dafs::Endpoint
                {
                    dafs::Address("1.1.1.1", 1000),
                    dafs::Address("1.1.1.1", 1111)
                },
                dafs::Endpoint
                {
                    dafs::Address("2.2.2.2", 2000),
                    dafs::Address("2.2.2.2", 2222)
                },
                dafs::Identity("22222222-2222-2222-2222-222222222222")
            ),
            std::make_shared<MockPartition>(
                dafs::Endpoint
                {
                    dafs::Address("2.2.2.2", 2000),
                    dafs::Address("2.2.2.2", 2222)
                },
                dafs::Endpoint
                {
                    dafs::Address("3.3.3.3", 3000),
                    dafs::Address("3.3.3.3", 3333)
                },
                dafs::Endpoint
                {
                    dafs::Address("1.1.1.1", 1000),
                    dafs::Address("1.1.1.1", 1111)
                },
                dafs::Identity("33333333-3333-3333-3333-333333333333")
            )
        );
    }

    std::shared_ptr<dafs::Node> node_under_test;

public:

    dafs::Node& GetNode()
    {
        return *node_under_test;
    }
};


void ASSERT_ADDRESS_EQUAL(dafs::Address a, dafs::Address b)
{
    ASSERT_EQ(a.ip, b.ip);
    ASSERT_EQ(a.port, b.port);
}


TEST_F(HandlerTest, testHandleReadBlock)
{
    dafs::BlockInfo info
    {
        "path/to/blockinfo",
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        0
    };
    dafs::BlockFormat format
    {
        "this is the content of the block format.",
    };

    GetNode().GetPartition(
        dafs::Identity("22222222-2222-2222-2222-222222222222")
    )->WriteBlock(info, format);

    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize(info)
            }
        }
    );
    dafs::Message m = HandleReadBlock(GetNode(), parser);
    dafs::BlockFormat result(
        dafs::MetaDataParser(m.metadata).GetValue<dafs::BlockFormat>(
            dafs::BlockFormatKey)
    );

    ASSERT_EQ(format.contents, result.contents);
}


TEST_F(HandlerTest, testHandleWriteBlock)
{
    dafs::BlockInfo info
    {
        "path/to/blockinfo",
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        0
    };
    dafs::BlockFormat format
    {
        "this is the content of the block format.",
    };

    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize(info)
            },
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::Serialize(format)
            }
        }
    );
    HandleWriteBlock(GetNode(), parser);

    ASSERT_EQ(
        format.contents,
        GetNode().GetPartition(dafs::Identity("22222222-2222-2222-2222-222222222222"))
                 ->ReadBlock(info).contents);
}


TEST_F(HandlerTest, testHandleDeleteBlock)
{
    dafs::BlockInfo info
    {
        "path/to/blockinfo",
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        0
    };
    dafs::BlockFormat format
    {
        "this is the content of the block format.",
    };

    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize(info)
            },
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::Serialize(format)
            }
        }
    );
    HandleWriteBlock(GetNode(), parser);
    HandleDeleteBlock(GetNode(), parser);

    ASSERT_EQ(
        "",
        GetNode().GetPartition(dafs::Identity("22222222-2222-2222-2222-222222222222"))
                 ->ReadBlock(info).contents);
}


TEST_F(HandlerTest, testGetNodeDetails)
{
    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
        }
    );
    MockSender mock_sender;
    dafs::Message m = HandleGetNodeDetails(GetNode(), parser);

    auto parsed = dafs::MetaDataParser(m.metadata);
    auto m_identity = parsed.GetValue<dafs::Identity>(dafs::MinusIdentityKey);
    auto z_identity = parsed.GetValue<dafs::Identity>(dafs::ZeroIdentityKey);
    auto p_identity = parsed.GetValue<dafs::Identity>(dafs::PlusIdentityKey);

    auto m_endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::MinusReplicatedEndpointsKey);
    auto z_endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::ZeroReplicatedEndpointsKey);
    auto p_endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::PlusReplicatedEndpointsKey);

    ASSERT_EQ(
        dafs::Identity("11111111-1111-1111-1111-111111111111"),
        m_identity);
    ASSERT_EQ(
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        z_identity);
    ASSERT_EQ(
        dafs::Identity("33333333-3333-3333-3333-333333333333"),
        p_identity);

    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), m_endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), m_endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2000), m_endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2222), m_endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), m_endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), m_endpoints.plus.replication);

    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), z_endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), z_endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), z_endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), z_endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2000), z_endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2222), z_endpoints.plus.replication);

    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2000), p_endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2222), p_endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), p_endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), p_endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), p_endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), p_endpoints.plus.replication);
}


TEST_F(HandlerTest, testHandleJoinCluster)
{
    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::AddressKey,
                dafs::Serialize(dafs::Address("A.B.C.D", 1234))
            }
        }
    );
    MockSender mock_sender;
    HandleJoinCluster(GetNode(), parser, mock_sender);

    dafs::Message sent_message = mock_sender.sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_RequestJoinCluster, sent_message.type);

    auto parsed = dafs::MetaDataParser(sent_message.metadata);
    auto endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::NodeEndpointsKey);
    auto identity = parsed.GetValue<dafs::Identity>(dafs::IdentityKey);

    ASSERT_EQ(
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        identity);

    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2000), endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("2.2.2.2", 2222), endpoints.plus.replication);
}


TEST_F(HandlerTest, testHandleMinusInitiationWithOutOfOrderIdentity)
{
    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::IdentityKey,
                dafs::Serialize(
                    dafs::Identity("99999999-9999-9999-9999-999999999999")
                )
            },
            dafs::MetaData
            {
                dafs::NodeEndpointsKey,
                dafs::Serialize(
                    dafs::ReplicatedEndpoints
                    {
                    }
                )
            }
        }
    );
    MockSender mock_sender;
    HandleRequestJoinCluster(GetNode(), parser, mock_sender);

    dafs::Message sent_message = mock_sender.sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_JoinCluster, sent_message.type);
}


TEST_F(HandlerTest, testHandleMinusInitiationWithActivePartition)
{
    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::IdentityKey,
                dafs::Serialize(
                    dafs::Identity("11111111-1111-1111-5555-555555555555")
                )
            },
            dafs::MetaData
            {
                dafs::NodeEndpointsKey,
                dafs::Serialize(
                    dafs::ReplicatedEndpoints
                    {
                    }
                )
            }
        }
    );
    MockSender mock_sender;
    HandleRequestJoinCluster(GetNode(), parser, mock_sender);

    dafs::Message sent_message = mock_sender.sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_AcceptJoinCluster, sent_message.type);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), sent_message.from);
}
