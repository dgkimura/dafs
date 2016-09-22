#include "gtest/gtest.h"

#include "serialization.hpp"


TEST(SerializationUnitTest, testDeltaIsSerializableAndDeserializable)
{
    dafs::Delta expected
    {
        "filename-string",
        "difference-string"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Delta>(string_obj);

    ASSERT_EQ(expected.filename, actual.filename);
    ASSERT_EQ(expected.difference, actual.difference);
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
