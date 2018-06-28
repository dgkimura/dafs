#include "gtest/gtest.h"

#include "dafs/serialization.hpp"


TEST(SerializationUnitTest, testDeltaIsSerializableAndDeserializable)
{
    dafs::Delta expected
    {
        "the-difference"
    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Delta>(string_obj);

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


TEST(SerializationUnitTest, testEndpointIsSerializableAndDeserializable)
{
    dafs::Endpoint expected
    {
        dafs::Address
        {
            "1.1.1.1", 1111
        },
        dafs::Address
        {
            "2.2.2.2", 2222
        },
        dafs::Identity("33333333-3333-3333-3333-333333333333")

    }, actual;

    std::string string_obj = dafs::Serialize(expected);
    actual = dafs::Deserialize<dafs::Endpoint>(string_obj);

    ASSERT_EQ(expected.management.ip, actual.management.ip);
    ASSERT_EQ(expected.management.port, actual.management.port);
    ASSERT_EQ(expected.replication.ip, actual.replication.ip);
    ASSERT_EQ(expected.replication.port, actual.replication.port);
    ASSERT_EQ(expected.identity, actual.identity);
}


TEST(SerializationUnitTest, _testIdentityIsSerializableAndDeserializable)
{
    dafs::Identity expected("33333333-3333-3333-3333-333333333333");

    std::string string_obj = dafs::serialize(expected);
    dafs::Identity actual = dafs::deserialize<dafs::Identity>(string_obj);

    ASSERT_EQ(expected.id, actual.id);
}


TEST(SerializationUnitTest, _testBlockFormatIsSerializableAndDeserializable)
{
    dafs::BlockFormat expected
    {
        "this is the block contents"
    }, actual;

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::BlockFormat>(string_obj);

    ASSERT_EQ(std::string(expected.contents), std::string(actual.contents));
}


TEST(SerializationUnitTest, _testBlockInfoIsSerializableAndDeserializable)
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

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::BlockInfo>(string_obj);

    ASSERT_EQ(expected.path, actual.path);
    ASSERT_EQ(expected.identity, actual.identity);
}


TEST(SerializationUnitTest, _testAddressIsSerializableAndDeserializable)
{
    dafs::Address expected
    {
        "the-ip-address",
        8080
    }, actual;

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::Address>(string_obj);

    ASSERT_EQ(std::string(expected.ip), std::string(actual.ip));
    ASSERT_EQ(expected.port, actual.port);
}


TEST(SerializationUnitTest, _testEndpointIsSerializableAndDeserializable)
{
    dafs::Endpoint expected
    {
        dafs::Address
        {
            "1.1.1.1", 1111
        },
        dafs::Address
        {
            "2.2.2.2", 2222
        },
        dafs::Identity("33333333-3333-3333-3333-333333333333")

    }, actual;

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::Endpoint>(string_obj);

    ASSERT_EQ(expected.management.ip, actual.management.ip);
    ASSERT_EQ(expected.management.port, actual.management.port);
    ASSERT_EQ(expected.replication.ip, actual.replication.ip);
    ASSERT_EQ(expected.replication.port, actual.replication.port);
    ASSERT_EQ(expected.identity, actual.identity);
}


TEST(SerializationUnitTest, _testReplicatedEndpointsIsSerializableAndDeserializable)
{
    dafs::ReplicatedEndpoints expected
    {
        dafs::Endpoint
        {
            dafs::Address
            {
                "1.1.1.1", 1111
            },
            dafs::Address
            {
                "2.2.2.2", 2222
            },
            dafs::Identity("11111111-1111-1111-1111-111111111111")

        },
        dafs::Endpoint
        {
            dafs::Address
            {
                "3.3.3.3", 3333
            },
            dafs::Address
            {
                "4.4.4.4", 4444
            },
            dafs::Identity("22222222-2222-2222-2222-222222222222")

        },
        dafs::Endpoint
        {
            dafs::Address
            {
                "5.5.5.5", 5555
            },
            dafs::Address
            {
                "6.6.6.6", 6666
            },
            dafs::Identity("33333333-3333-3333-3333-333333333333")

        },
    }, actual;

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::ReplicatedEndpoints>(string_obj);

    ASSERT_EQ(expected.minus.management.ip, actual.minus.management.ip);
    ASSERT_EQ(expected.minus.management.port, actual.minus.management.port);
    ASSERT_EQ(expected.minus.replication.ip, actual.minus.replication.ip);
    ASSERT_EQ(expected.minus.replication.port, actual.minus.replication.port);
    ASSERT_EQ(expected.minus.identity, actual.minus.identity);

    ASSERT_EQ(expected.zero.management.ip, actual.zero.management.ip);
    ASSERT_EQ(expected.zero.management.port, actual.zero.management.port);
    ASSERT_EQ(expected.zero.replication.ip, actual.zero.replication.ip);
    ASSERT_EQ(expected.zero.replication.port, actual.zero.replication.port);
    ASSERT_EQ(expected.zero.identity, actual.zero.identity);

    ASSERT_EQ(expected.plus.management.ip, actual.plus.management.ip);
    ASSERT_EQ(expected.plus.management.port, actual.plus.management.port);
    ASSERT_EQ(expected.plus.replication.ip, actual.plus.replication.ip);
    ASSERT_EQ(expected.plus.replication.port, actual.plus.replication.port);
    ASSERT_EQ(expected.plus.identity, actual.plus.identity);
}


TEST(SerializationUnitTest, _testMetaDataIsSerializableAndDeserializable)
{
    dafs::MetaData expected
    {
        "Address", // must be valid metadata key
        "value-string"
    }, actual;

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::MetaData>(string_obj);

    ASSERT_EQ(expected.key, actual.key);
    ASSERT_EQ(expected.value, actual.value);
}


TEST(SerializationUnitTest, _testMessageIsSerializableAndDeserializable)
{
    dafs::Message expected
    {
        dafs::MessageType::ReadBlock,
        {
            {
                "BlockInfo",
                "block-info-value"
            },
            {
                "BlockFormat",
                "block-format-value"
            }
        }
    }, actual;

    std::string string_obj = dafs::serialize(expected);
    actual = dafs::deserialize<dafs::Message>(string_obj);

    ASSERT_EQ(expected.type, actual.type);
    ASSERT_EQ(expected.metadata[0].key, actual.metadata[0].key);
    ASSERT_EQ(expected.metadata[0].value, actual.metadata[0].value);
    ASSERT_EQ(expected.metadata[1].key, actual.metadata[1].key);
    ASSERT_EQ(expected.metadata[1].value, actual.metadata[1].value);
}
