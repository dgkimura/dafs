#include "gtest/gtest.h"

#include "dafs/filesystem.hpp"


TEST(FileSystemTest, testBlockFieldsAreConstructed)
{
    std::string contents = "the_block_contents";

    dafs::BlockFormat b;
    b.contents = contents;

    ASSERT_EQ(contents, b.contents);
}


TEST(FileSystemTest, testBytesFieldsAreConstructed)
{
    std::string data = "the_bytes_contents";

    dafs::Bytes b = dafs::CreateBytes(data);

    ASSERT_EQ(data, b.contents);
}


TEST(FileSystemTest, testLocationFieldsAreConstructed)
{
    std::string address = "localhost";

    dafs::Location l = dafs::CreateLocation(address);

    ASSERT_EQ(address, l.address);
}


TEST(FileSystemTest, testsBlockInfoFieldsAreConstructed)
{
    std::string path = "myfile";
    dafs::Location location = dafs::CreateLocation("localhost");

    dafs::BlockInfo b = dafs::CreateBlockInfo(path, location);

    ASSERT_EQ(path, b.path);
    ASSERT_EQ(location.address, b.location.address);
}
