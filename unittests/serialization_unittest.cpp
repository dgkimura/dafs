#include "gtest/gtest.h"

#include "serialization.hpp"


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
        dafs::Location
        {
            "the-address",
            80
        },
        12, //offset
        23  //revision
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::BlockInfo>(string_obj);

    ASSERT_EQ(expected.path, actual.path);
    ASSERT_EQ(expected.location.address, actual.location.address);
    ASSERT_EQ(expected.offset, actual.offset);
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
        dafs::Location
        {
            "previous",
            80
        },
        dafs::Location
        {
            "next",
            80
        },
        1,
        "filename"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileInfo>(string_obj);

    ASSERT_EQ(expected.previous.address, actual.previous.address);
    ASSERT_EQ(expected.next.address, actual.next.address);
    ASSERT_EQ(expected.descriptor, actual.descriptor);
    ASSERT_EQ(expected.path, actual.path);
}


TEST(SerializationUnitTest, testFileFormatIsSerializableAndDeserializable)
{
    dafs::FileFormat expected
    {
        dafs::FileInfo
        {
            dafs::Location
            {
                "previous",
                80
            },
            dafs::Location
            {
                "next",
                80
            },
            1
        },
        {
            dafs::BlockInfo
            {
                "the-path-of-block-1",
                dafs::Location
                {
                    "the-address",
                    80
                }
            },
            dafs::BlockInfo
            {
                "the-path-of-block-2",
                dafs::Location
                {
                    "the-address",
                    80
                }
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileFormat>(string_obj);

    ASSERT_EQ(expected.info.previous.address, actual.info.previous.address);
    ASSERT_EQ(expected.info.next.address, actual.info.next.address);
    ASSERT_EQ(expected.info.descriptor, actual.info.descriptor);
    ASSERT_EQ(expected.info.path, actual.info.path);
    ASSERT_EQ(expected.blocks[0].path, actual.blocks[0].path);
    ASSERT_EQ(expected.blocks[0].location.address, actual.blocks[0].location.address);
    ASSERT_EQ(expected.blocks[1].path, actual.blocks[1].path);
    ASSERT_EQ(expected.blocks[1].location.address, actual.blocks[1].location.address);
}


TEST(SerializationUnitTest, testFileIndexIsSerializableAndDeserializable)
{
    dafs::FileIndex expected
    {
        std::vector<dafs::FileInfo>
        {
            dafs::FileInfo
            {
                dafs::Location
                {
                    "previous",
                    80
                },
                dafs::Location
                {
                    "next",
                    80
                },
                1,
                "filename"
            },
            dafs::FileInfo
            {
                dafs::Location
                {
                    "my-previous",
                    80
                },
                dafs::Location
                {
                    "my-next",
                    80
                },
                2,
                "my-file"
            },
            dafs::FileInfo
            {
                dafs::Location
                {
                    "your-previous",
                    80
                },
                dafs::Location
                {
                    "your-next",
                    80
                },
                3,
                "your-file"
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileIndex>(string_obj);

    ASSERT_EQ(expected.files[0].path, actual.files[0].path);
    ASSERT_EQ(expected.files[1].path, actual.files[1].path);
    ASSERT_EQ(expected.files[2].path, actual.files[2].path);
}


TEST(SerializationUnitTest, testBlockIndexIsSerializableAndDeserializable)
{
    dafs::BlockIndex expected
    {
        std::vector<dafs::BlockInfo>
        {
            dafs::BlockInfo
            {
                "my-block",
                dafs::Location
                {
                    "my-address",
                    80
                },
                0, // offset
                0  // revision
            },
            dafs::BlockInfo
            {
                "your-block",
                dafs::Location
                {
                    "your-address",
                    80
                },
                1, // offset
                1  // revision
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::BlockIndex>(string_obj);

    ASSERT_EQ(expected.blocks[0].path, actual.blocks[0].path);
    ASSERT_EQ(expected.blocks[1].location.address, actual.blocks[1].location.address);
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
        "to",
        "from",
        "the message content",
        dafs::MessageType::CreateFile,
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

    ASSERT_EQ(expected.to, actual.to);
    ASSERT_EQ(expected.from, actual.from);
    ASSERT_EQ(expected.content, actual.content);
    ASSERT_EQ(expected.type, actual.type);
    ASSERT_EQ(expected.metadata[0].key, actual.metadata[0].key);
    ASSERT_EQ(expected.metadata[0].value, actual.metadata[0].value);
    ASSERT_EQ(expected.metadata[1].key, actual.metadata[1].key);
    ASSERT_EQ(expected.metadata[1].value, actual.metadata[1].value);
}


TEST(SerializationUnitTest, testNodeSetIsSerializableAndDeserializable)
{
    dafs::NodeSet expected
    {
        std::vector<std::string>
        {
            { "address-1:80" },
            { "address-2:80" }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::NodeSet>(string_obj);

    ASSERT_EQ(expected.endpoints[0], actual.endpoints[0]);
    ASSERT_EQ(expected.endpoints[1], actual.endpoints[1]);
}
