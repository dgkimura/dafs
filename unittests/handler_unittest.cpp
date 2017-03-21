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
                dafs::Address("1.1.1.1", 1000),
                dafs::Address("1.1.1.1", 1001),
                dafs::Identity("11111111-1111-1111-1111-111111111111")
            ),
            std::make_shared<MockPartition>(
                dafs::Address("2.2.2.2", 1000),
                dafs::Address("2.2.2.2", 1001),
                dafs::Identity("22222222-2222-2222-2222-222222222222")
            ),
            std::make_shared<MockPartition>(
                dafs::Address("3.3.3.3", 1000),
                dafs::Address("3.3.3.3", 1001),
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


TEST_F(HandlerTest, testHandleGetNodeDetails)
{
    dafs::MetaDataParser parser(
        std::vector<dafs::MetaData>
        {
        }
    );
    dafs::Message m = HandleGetNodeDetails(GetNode(), parser);

    dafs::NodeDetails result(
        dafs::MetaDataParser(m.metadata).GetValue<dafs::NodeDetails>(
            dafs::NodeDetailsKey));

    ASSERT_EQ(
        dafs::Identity("11111111-1111-1111-1111-111111111111"),
        result.minus_details.identity);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1000).ip,
        result.minus_details.author.ip);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1000).port,
        result.minus_details.author.port);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1001).ip,
        result.minus_details.interface.ip);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1001).port,
        result.minus_details.interface.port);
    ASSERT_EQ(
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        result.zero_details.identity);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1000).ip,
        result.zero_details.author.ip);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1000).port,
        result.zero_details.author.port);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1001).ip,
        result.zero_details.interface.ip);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1001).port,
        result.zero_details.interface.port);
    ASSERT_EQ(
        dafs::Identity("33333333-3333-3333-3333-333333333333"),
        result.plus_details.identity);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1000).ip,
        result.plus_details.author.ip);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1000).port,
        result.plus_details.author.port);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1001).ip,
        result.plus_details.interface.ip);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1001).port,
        result.plus_details.interface.port);
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

    ASSERT_EQ("2.2.2.2", sent_message.from.ip);
    ASSERT_EQ(1000, sent_message.from.port);
    ASSERT_EQ("A.B.C.D", sent_message.to.ip);
    ASSERT_EQ(1234, sent_message.to.port);
    ASSERT_EQ(dafs::MessageType::_RequestMinusInitiation, sent_message.type);

    auto result = dafs::MetaDataParser(sent_message.metadata).GetValue<dafs::NodeDetails>(dafs::NodeDetailsKey);

    ASSERT_EQ(
        dafs::Identity("11111111-1111-1111-1111-111111111111"),
        result.minus_details.identity);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1000).ip,
        result.minus_details.author.ip);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1000).port,
        result.minus_details.author.port);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1001).ip,
        result.minus_details.interface.ip);
    ASSERT_EQ(
        dafs::Address("1.1.1.1", 1001).port,
        result.minus_details.interface.port);
    ASSERT_EQ(
        dafs::Identity("22222222-2222-2222-2222-222222222222"),
        result.zero_details.identity);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1000).ip,
        result.zero_details.author.ip);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1000).port,
        result.zero_details.author.port);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1001).ip,
        result.zero_details.interface.ip);
    ASSERT_EQ(
        dafs::Address("2.2.2.2", 1001).port,
        result.zero_details.interface.port);
    ASSERT_EQ(
        dafs::Identity("33333333-3333-3333-3333-333333333333"),
        result.plus_details.identity);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1000).ip,
        result.plus_details.author.ip);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1000).port,
        result.plus_details.author.port);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1001).ip,
        result.plus_details.interface.ip);
    ASSERT_EQ(
        dafs::Address("3.3.3.3", 1001).port,
        result.plus_details.interface.port);
}
