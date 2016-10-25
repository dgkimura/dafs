#include "gtest/gtest.h"

#include "filesystem.hpp"


TEST(FileSystemTest, testBlockFieldsAreConstructed)
{
    std::string contents = "the_block_contents";

    dafs::BlockFormat b;
    strcpy(b.contents, contents.c_str());

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
    std::string filename = "myfile";
    dafs::Location location = dafs::CreateLocation("localhost");

    dafs::BlockInfo b = dafs::CreateBlockInfo(filename, location);

    ASSERT_EQ(filename, b.filename);
    ASSERT_EQ(location.address, b.location.address);
    ASSERT_EQ(0, b.offset);
}
