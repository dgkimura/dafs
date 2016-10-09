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
        "the-filename",
        dafs::Location
        {
            "the-address"
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::BlockInfo>(string_obj);

    ASSERT_EQ(expected.filename, actual.filename);
    ASSERT_EQ(expected.location.address, actual.location.address);
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
        "previous",
        "next",
        1,
        "filename"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileInfo>(string_obj);

    ASSERT_EQ(expected.previous.address, actual.previous.address);
    ASSERT_EQ(expected.next.address, actual.next.address);
    ASSERT_EQ(expected.descriptor, actual.descriptor);
    ASSERT_EQ(expected.name, actual.name);
}


TEST(SerializationUnitTest, testFileFormatIsSerializableAndDeserializable)
{
    dafs::FileFormat expected
    {
        {
            "previous",
            "next",
            1
        },
        {
            {
                "the-filename-of-block-1",
                dafs::Location
                {
                    "the-address"
                }
            },
            {
                "the-filename-of-block-2",
                dafs::Location
                {
                    "the-address"
                }
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileFormat>(string_obj);

    ASSERT_EQ(expected.info.previous.address, actual.info.previous.address);
    ASSERT_EQ(expected.info.next.address, actual.info.next.address);
    ASSERT_EQ(expected.info.descriptor, actual.info.descriptor);
    ASSERT_EQ(expected.info.name, actual.info.name);
    ASSERT_EQ(expected.blocks[0].filename, actual.blocks[0].filename);
    ASSERT_EQ(expected.blocks[0].location.address, actual.blocks[0].location.address);
    ASSERT_EQ(expected.blocks[1].filename, actual.blocks[1].filename);
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
        dafs::MessageType::ReadFile,
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
