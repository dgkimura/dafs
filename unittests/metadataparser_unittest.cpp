#include "gtest/gtest.h"

#include "dafs/filesystem.hpp"
#include "dafs/metadataparser.hpp"


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
                    dafs::Identity
                    {
                        "00000000-0000-0000-0000-000000000000",
                    },
                    dafs::Identity
                    {
                        "11111111-1111-1111-1111-111111111111",
                    },
                    1,
                    "name"
                }
            )
        }
    };

    dafs::MetaDataParser parser(metadata);

    ASSERT_EQ(parser.GetValue<dafs::FileInfo>(dafs::FileInfoKey).path, "name");
}
