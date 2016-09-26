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
        "the-address"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::BlockInfo>(string_obj);

    ASSERT_EQ(expected.filename, actual.filename);
    ASSERT_EQ(expected.address, actual.address);
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
        1
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileInfo>(string_obj);

    ASSERT_EQ(expected.previous, actual.previous);
    ASSERT_EQ(expected.next, actual.next);
    ASSERT_EQ(expected.identifier, actual.identifier);
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
                "the-address"
            },
            {
                "the-filename-of-block-2",
                "the-address"
            }
        }
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::FileFormat>(string_obj);

    ASSERT_EQ(expected.info.previous, actual.info.previous);
    ASSERT_EQ(expected.info.next, actual.info.next);
    ASSERT_EQ(expected.info.identifier, actual.info.identifier);
    ASSERT_EQ(expected.blocks[0].filename, actual.blocks[0].filename);
    ASSERT_EQ(expected.blocks[0].address, actual.blocks[0].address);
    ASSERT_EQ(expected.blocks[1].filename, actual.blocks[1].filename);
    ASSERT_EQ(expected.blocks[1].address, actual.blocks[1].address);
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
    ASSERT_EQ(expected.metadata[0].key, actual.metadata[0].key);
    ASSERT_EQ(expected.metadata[0].value, actual.metadata[0].value);
    ASSERT_EQ(expected.metadata[1].key, actual.metadata[1].key);
    ASSERT_EQ(expected.metadata[1].value, actual.metadata[1].value);
}
