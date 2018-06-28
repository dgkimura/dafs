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
                    dafs::Address("1.1.1.1", 1111),
                    dafs::Identity("11111111-1111-1111-1111-111111111111")
                },
                dafs::Endpoint
                {
                    dafs::Address("3.3.3.3", 3000),
                    dafs::Address("3.3.3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333")
                },
                dafs::Endpoint
                {
                    dafs::Address("5.5.5.5", 5000),
                    dafs::Address("5.5.5.5", 5555),
                    dafs::Identity("55555555-5555-5555-5555-555555555555")
                }
            ),
            std::make_shared<MockPartition>(
                dafs::Endpoint
                {
                    dafs::Address("5.5.5.5", 5000),
                    dafs::Address("5.5.5.5", 5555),
                    dafs::Identity("55555555-5555-5555-5555-555555555555")
                },
                dafs::Endpoint
                {
                    dafs::Address("1.1.1.1", 1000),
                    dafs::Address("1.1.1.1", 1111),
                    dafs::Identity("11111111-1111-1111-1111-111111111111")
                },
                dafs::Endpoint
                {
                    dafs::Address("3.3.3.3", 3000),
                    dafs::Address("3.3.3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333")
                }
            ),
            std::make_shared<MockPartition>(
                dafs::Endpoint
                {
                    dafs::Address("3.3.3.3", 3000),
                    dafs::Address("3.3.3.3", 3333),
                    dafs::Identity("33333333-3333-3333-3333-333333333333")
                },
                dafs::Endpoint
                {
                    dafs::Address("5.5.5.5", 5000),
                    dafs::Address("5.5.5.5", 5555),
                    dafs::Identity("55555555-5555-5555-5555-555555555555")
                },
                dafs::Endpoint
                {
                    dafs::Address("7.7.7.7", 7000),
                    dafs::Address("7.7.7.7", 7777),
                    dafs::Identity("77777777-7777-7777-7777-777777777777")
                }
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
        dafs::Identity("33333333-3333-3333-3333-333333333333"),
        0
    };
    dafs::BlockFormat format
    {
        "this is the content of the block format.",
    };

    GetNode().GetPartition(
        dafs::Identity("33333333-3333-3333-3333-333333333333")
    )->WriteBlock(info, format);

    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::ReadBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::serialize(info)
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleReadBlock(GetNode(), message, mock_sender);
    auto m = mock_sender->sentMessages()[0];
    dafs::BlockFormat result(
        dafs::MetaDataParser(m.metadata).GetValue<dafs::BlockFormat>(
            dafs::BlockFormatKey)
    );

    ASSERT_EQ(format.contents, result.contents);
}


TEST_F(HandlerTest, testHandleReadBlockForwardsRequests)
{
    // identity does not exist on node
    dafs::BlockInfo info
    {
        "path/to/blockinfo",
        dafs::Identity("77777777-7777-7777-7777-777777777777"),
        0
    };

    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::ReadBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::serialize(info)
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleReadBlock(GetNode(), message, mock_sender);
    auto m = mock_sender->sentMessages()[0];
    dafs::Address result(
        dafs::MetaDataParser(m.metadata).GetValue<dafs::Address>(
            dafs::AddressKey)
    );

    ASSERT_EQ(dafs::MessageType::Failure, m.type);
    ASSERT_EQ("1.1.1.1", result.ip);
    ASSERT_EQ(1000, result.port);
}


TEST_F(HandlerTest, testHandleWriteBlock)
{
    dafs::BlockInfo info
    {
        "path/to/blockinfo",
        dafs::Identity("33333333-3333-3333-3333-333333333333"),
        0
    };
    dafs::BlockFormat format
    {
        "this is the content of the block format.",
    };

    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::WriteBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::serialize(info)
            },
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::serialize(format)
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleWriteBlock(GetNode(), message, mock_sender);

    ASSERT_EQ(
        format.contents,
        GetNode().GetPartition(dafs::Identity("33333333-3333-3333-3333-333333333333"))
                 ->ReadBlock(info).contents);
}


TEST_F(HandlerTest, testHandleWriteBlockForwardsRequests)
{

    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::WriteBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::serialize(
                    // identity does not belong on node
                    dafs::BlockInfo
                    {
                        "path/to/blockinfo",
                        dafs::Identity("77777777-7777-7777-7777-777777777777"),
                        0
                    }
                )
            },
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::serialize(
                    dafs::BlockFormat
                    {
                        "this is the content of the block format.",
                    }
                )
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleWriteBlock(GetNode(), message, mock_sender);
    auto m = mock_sender->sentMessages()[0];
    dafs::Address result(
        dafs::MetaDataParser(m.metadata).GetValue<dafs::Address>(
            dafs::AddressKey)
    );

    ASSERT_EQ(dafs::MessageType::Failure, m.type);
    ASSERT_EQ("1.1.1.1", result.ip);
    ASSERT_EQ(1000, result.port);
}


TEST_F(HandlerTest, testHandleDeleteBlock)
{
    dafs::BlockInfo info
    {
        "path/to/blockinfo",
        dafs::Identity("33333333-3333-3333-3333-333333333333"),
        0
    };
    dafs::BlockFormat format
    {
        "this is the content of the block format.",
    };

    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::WriteBlock,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::serialize(info)
            },
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::serialize(format)
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleWriteBlock(GetNode(), message, mock_sender);
    HandleDeleteBlock(GetNode(), message, mock_sender);

    ASSERT_EQ(
        "",
        GetNode().GetPartition(dafs::Identity("33333333-3333-3333-3333-333333333333"))
                 ->ReadBlock(info).contents);
}


TEST_F(HandlerTest, testGetNodeDetails)
{
    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::GetNodeDetails,
        std::vector<dafs::MetaData>
        {
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleGetNodeDetails(GetNode(), message, mock_sender);
    auto m = mock_sender->sentMessages()[0];

    auto parsed = dafs::MetaDataParser(m.metadata);

    auto m_endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::MinusReplicatedEndpointsKey);
    auto z_endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::ZeroReplicatedEndpointsKey);
    auto p_endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::PlusReplicatedEndpointsKey);

    ASSERT_EQ(
        "33333333-3333-3333-3333-333333333333",
        m_endpoints.zero.identity.id);
    ASSERT_EQ(
        "11111111-1111-1111-1111-111111111111",
        z_endpoints.zero.identity.id);
    ASSERT_EQ(
        "55555555-5555-5555-5555-555555555555",
        p_endpoints.zero.identity.id);

    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), m_endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), m_endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), m_endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), m_endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5000), m_endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5555), m_endpoints.plus.replication);

    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5000), z_endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5555), z_endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), z_endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), z_endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), z_endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), z_endpoints.plus.replication);

    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), p_endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), p_endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5000), p_endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5555), p_endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("7.7.7.7", 7000), p_endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("7.7.7.7", 7777), p_endpoints.plus.replication);
}


TEST_F(HandlerTest, testHandleJoinCluster)
{
    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::_JoinCluster,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::AddressKey,
                dafs::serialize(dafs::Address("A.B.C.D", 1234))
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleJoinCluster(GetNode(), message, mock_sender);

    dafs::Message sent_message = mock_sender->sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_RequestJoinCluster, sent_message.type);

    auto parsed = dafs::MetaDataParser(sent_message.metadata);
    auto endpoints = parsed.GetValue<dafs::ReplicatedEndpoints>(dafs::NodeEndpointsKey);
    auto identity = parsed.GetValue<dafs::Identity>(dafs::IdentityKey);

    ASSERT_EQ("11111111-1111-1111-1111-111111111111", identity.id);

    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5000), endpoints.minus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("5.5.5.5", 5555), endpoints.minus.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), endpoints.zero.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1111), endpoints.zero.replication);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), endpoints.plus.management);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3333), endpoints.plus.replication);
}


TEST_F(HandlerTest, testHandleMinusInitiationWithOutOfOrderIdentity)
{
    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::_RequestJoinCluster,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::IdentityKey,
                dafs::serialize(
                    dafs::Identity("22222222-2222-2222-2222-222222222222")
                )
            },
            dafs::MetaData
            {
                dafs::NodeEndpointsKey,
                dafs::serialize(
                    dafs::ReplicatedEndpoints
                    {
                    }
                )
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleRequestJoinCluster(GetNode(), message, mock_sender);

    dafs::Message sent_message = mock_sender->sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_JoinCluster, sent_message.type);
}


TEST_F(HandlerTest, testHandleMinusInitiationWithActivePartition)
{
    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::_RequestJoinCluster,
        std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::IdentityKey,
                dafs::serialize(
                    dafs::Identity("00000000-0000-0000-0000-000000000000")
                )
            },
            dafs::MetaData
            {
                dafs::NodeEndpointsKey,
                dafs::serialize(
                    dafs::ReplicatedEndpoints
                    {
                    }
                )
            }
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleRequestJoinCluster(GetNode(), message, mock_sender);

    dafs::Message sent_message = mock_sender->sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_AcceptJoinCluster, sent_message.type);
    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), sent_message.from);
}


TEST_F(HandlerTest, testHandleExitClusterWithActivePartition)
{
    dafs::Message message
    {
        dafs::Address
        {
            "from-ip",
            1111
        },
        dafs::Address
        {
            "to-ip",
            1111
        },
        dafs::MessageType::_PlusExitCluster,
        std::vector<dafs::MetaData>
        {
        }
    };
    auto mock_sender = std::make_shared<MockSender>();
    HandleExitCluster(GetNode(), message, mock_sender);

    dafs::Message sent_message = mock_sender->sentMessages()[0];

    ASSERT_EQ(dafs::MessageType::_PlusExitCluster, sent_message.type);
    ASSERT_EQ("11111111-1111-1111-1111-111111111111",
              dafs::MetaDataParser(sent_message.metadata).GetValue<dafs::Identity>(dafs::IdentityKey).id);

    ASSERT_ADDRESS_EQUAL(dafs::Address("1.1.1.1", 1000), sent_message.from);
    ASSERT_ADDRESS_EQUAL(dafs::Address("3.3.3.3", 3000), sent_message.to);
}
