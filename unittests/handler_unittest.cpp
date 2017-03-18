#include <unordered_map>

#include <dafs/handler.hpp>
#include <dafs/node.hpp>
#include <gtest/gtest.h>

#include "mock_partition.hpp"


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
