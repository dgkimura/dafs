#include "gtest/gtest.h"

#include "messages.hpp"


TEST(MessagesTest, testMetaDataParserFromEmptyMetaDataList)
{
    std::vector<dafs::MetaData> metadata
    {
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue("MyKey"), "");
}


TEST(MessagesTest, testMetaDataParserCanGetValue)
{
    std::vector<dafs::MetaData> metadata
    {
        dafs::MetaData { "YourKey", "YourValue" },
        dafs::MetaData { "MyKey", "MyValue" }
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue("MyKey"), "MyValue");
}
