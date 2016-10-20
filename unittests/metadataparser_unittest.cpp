#include "gtest/gtest.h"

#include "filesystem.hpp"
#include "metadataparser.hpp"


TEST(MetaDataParserTest, testMetaDataParserFromEmptyMetaDataList)
{
    std::vector<dafs::MetaData> metadata
    {
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue<dafs::FileInfo>(dafs::FileInfoKey), nullptr);
}


TEST(MetaDataParserTest, testMetaDataParserUsingNonexistantKey)
{
    std::vector<dafs::MetaData> metadata
    {
        dafs::MetaData
        {
            "ExistingKey",
            dafs::Serialize(dafs::Location{"an_address"})
        }
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue<dafs::FileInfo>("NonexistingKey"), nullptr);
}


TEST(MessagesTest, testMetaDataParserCanGetValue)
{
    std::vector<dafs::MetaData> metadata
    {
        dafs::MetaData
        {
            dafs::FileInfoKey,
            dafs::Serialize<dafs::FileInfo>(
                dafs::FileInfo
                {
                    dafs::Location
                    {
                        "previous"
                    },
                    dafs::Location
                    {
                        "next"
                    },
                    1,
                    "name"
                }
            )
        }
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue<dafs::FileInfo>(dafs::FileInfoKey)->name, "name");
}
