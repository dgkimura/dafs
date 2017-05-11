#include "gtest/gtest.h"

#include "dafs/serialization.hpp"


TEST(SerializationUnitTest, testDeltaIsSerializableAndDeserializable)
{
    dafs::Delta expected
    {
        "the-filename",
        "the-difference"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Delta>(string_obj);

    ASSERT_EQ(expected.filename, actual.filename);
    ASSERT_EQ(expected.difference, actual.difference);
}


TEST(SerializationUnitTest, testBlockInfoIsSerializableAndDeserializable)
{
    dafs::BlockInfo expected
    {
        "the-path",
        dafs::Identity
        {
            "00000000-0000-0000-0000-000000000000",
        },
        23  //revision
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::BlockInfo>(string_obj);

    ASSERT_EQ(expected.path, actual.path);
    ASSERT_EQ(expected.identity, actual.identity);
}


TEST(SerializationUnitTest, testBytesIsSerializableAndDeserializable)
{
    dafs::Bytes expected
    {
        "this is the bytes contents"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Bytes>(string_obj);

    ASSERT_EQ(std::string(expected.contents), std::string(actual.contents));
}


TEST(SerializationUnitTest, testBlockFormatIsSerializableAndDeserializable)
{
    dafs::BlockFormat expected
    {
        "this is the block contents"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::BlockFormat>(string_obj);

    ASSERT_EQ(std::string(expected.contents), std::string(actual.contents));
}


TEST(SerializationUnitTest, testFileInfoIsSerializableAndDeserializable)
{
    dafs::FileInfo expected
    {
        dafs::Identity
        {
            "00000000-0000-0000-0000-000000000000",
        },
        dafs::Identity
        {
            "22222222-2222-2222-2222-222222222222",
        },
        dafs::Identity
        {
            "33333333-3333-3333-3333-333333333333",
        },
        "filename"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileInfo>(string_obj);

    ASSERT_EQ(expected.current, actual.current);
    ASSERT_EQ(expected.previous, actual.previous);
    ASSERT_EQ(expected.next, actual.next);
    ASSERT_EQ(expected.path, actual.path);
}


TEST(SerializationUnitTest, testFileFormatIsSerializableAndDeserializable)
{
    dafs::FileFormat expected
    {
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
            dafs::Identity
            {
                "33333333-3333-3333-3333-333333333333",
            },
            "filename"
        },
        {
            dafs::BlockInfo
            {
                "the-path-of-block-1",
                dafs::Identity
                {
                    "00000000-0000-0000-0000-000000000000",
                }
            },
            dafs::BlockInfo
            {
                "the-path-of-block-2",
                dafs::Identity
                {
                    "11111111-1111-1111-1111-111111111111",
                }
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileFormat>(string_obj);

    ASSERT_EQ(expected.info.current, actual.info.current);
    ASSERT_EQ(expected.info.previous, actual.info.previous);
    ASSERT_EQ(expected.info.next, actual.info.next);
    ASSERT_EQ(expected.info.path, actual.info.path);
    ASSERT_EQ(expected.blocks[0].path, actual.blocks[0].path);
    ASSERT_EQ(expected.blocks[0].identity, actual.blocks[0].identity);
    ASSERT_EQ(expected.blocks[1].path, actual.blocks[1].path);
    ASSERT_EQ(expected.blocks[1].identity, actual.blocks[1].identity);
}


TEST(SerializationUnitTest, testFileIndexIsSerializableAndDeserializable)
{
    dafs::Index<dafs::FileInfo> expected
    {
        std::vector<dafs::FileInfo>
        {
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
                dafs::Identity
                {
                    "33333333-3333-3333-3333-333333333333",
                },
                "filename"
            },
            dafs::FileInfo
            {
                dafs::Identity
                {
                    "44444444-4444-4444-4444-444444444444",
                },
                dafs::Identity
                {
                    "55555555-5555-5555-5555-555555555555",
                },
                dafs::Identity
                {
                    "66666666-6666-6666-6666-666666666666",
                },
                "my-file"
            },
            dafs::FileInfo
            {
                dafs::Identity
                {
                    "77777777-7777-7777-7777-777777777777",
                },
                dafs::Identity
                {
                    "88888888-8888-8888-8888-888888888888",
                },
                dafs::Identity
                {
                    "99999999-9999-9999-9999-999999999999",
                },
                "your-file"
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Index<dafs::FileInfo>>(string_obj);

    ASSERT_EQ(expected.items[0].path, actual.items[0].path);
    ASSERT_EQ(expected.items[1].path, actual.items[1].path);
    ASSERT_EQ(expected.items[2].path, actual.items[2].path);
}


TEST(SerializationUnitTest, testBlockIndexIsSerializableAndDeserializable)
{
    dafs::Index<dafs::BlockInfo> expected
    {
        std::vector<dafs::BlockInfo>
        {
            dafs::BlockInfo
            {
                "my-block",
                dafs::Identity
                {
                    "00000000-0000-0000-0000-000000000000",
                },
                0  // revision
            },
            dafs::BlockInfo
            {
                "your-block",
                dafs::Identity
                {
                    "11111111-1111-1111-1111-111111111111",
                },
                1  // revision
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Index<dafs::BlockInfo>>(string_obj);

    ASSERT_EQ(expected.items[0].path, actual.items[0].path);
    ASSERT_EQ(expected.items[1].identity, actual.items[1].identity);
}


TEST(SerializationUnitTest, testMetaDataIsSerializableAndDeserializable)
{
    dafs::MetaData expected
    {
        "key-string",
        "value-string"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::MetaData>(string_obj);

    ASSERT_EQ(expected.key, actual.key);
    ASSERT_EQ(expected.value, actual.value);
}


TEST(SerializationUnitTest, testMessageIsSerializableAndDeserializable)
{
    dafs::Message expected
    {
        dafs::Address
        {
            "1.1.1.1", 1111
        },
        dafs::Address
        {
            "2.2.2.2", 2222
        },
        dafs::MessageType::ReadBlock,
        {
            {
                "my-metadata-key",
                "my-metadata-value"
            },
            {
                "your-metadata-key",
                "your-metadata-value"
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Message>(string_obj);

    ASSERT_EQ(expected.to.ip, actual.to.ip);
    ASSERT_EQ(expected.to.port, actual.to.port);
    ASSERT_EQ(expected.from.ip, actual.from.ip);
    ASSERT_EQ(expected.from.port, actual.from.port);
    ASSERT_EQ(expected.type, actual.type);
    ASSERT_EQ(expected.metadata[0].key, actual.metadata[0].key);
    ASSERT_EQ(expected.metadata[0].value, actual.metadata[0].value);
    ASSERT_EQ(expected.metadata[1].key, actual.metadata[1].key);
    ASSERT_EQ(expected.metadata[1].value, actual.metadata[1].value);
}


TEST(SerializationUnitTest, testNodeSetIsSerializableAndDeserializable)
{
    dafs::Index<std::string> expected
    {
        std::vector<std::string>
        {
            { "address-1:80" },
            { "address-2:80" }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Index<std::string>>(string_obj);

    ASSERT_EQ(expected.items[0], actual.items[0]);
    ASSERT_EQ(expected.items[1], actual.items[1]);
}


TEST(SerializationUnitTest, testProposalIsSerializableAndDeserializable)
{
    boost::uuids::string_generator gen;
    dafs::Proposal expected
    {
        dafs::ProposalType::Ping,
        "the proposal content",
        gen("{11111111-2222-3333-4444-555555555555}")
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Proposal>(string_obj);

    ASSERT_EQ(expected.type, actual.type);
    ASSERT_EQ(expected.content, actual.content);
    ASSERT_EQ(expected.uuid, actual.uuid);
}
