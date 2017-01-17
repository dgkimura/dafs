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

    ASSERT_EQ(parser.GetValue<dafs::FileInfo>(dafs::FileInfoKey).path, "name");
}
