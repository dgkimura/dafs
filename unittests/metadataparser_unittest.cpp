#include "gtest/gtest.h"

#include "dafs/filesystem.hpp"
#include "dafs/metadataparser.hpp"


TEST(MessagesTest, testMetaDataParserCanGetValue)
{
    std::vector<dafs::MetaData> metadata
    {
        dafs::MetaData
        {
            dafs::IdentityKey,
            dafs::Serialize<dafs::Identity>(
                dafs::Identity
                {
                    "11111111-1111-1111-1111-111111111111",
                }
            )
        }
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue<dafs::Identity>(dafs::IdentityKey).id, "11111111-1111-1111-1111-111111111111");
}
