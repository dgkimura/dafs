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


TEST(FileSystemTest, testsBlockInfoFieldsAreConstructed)
{
    std::string path = "myfile";
    dafs::Identity identity = dafs::Identity("01234567-89ab-cdef-0123-456789abcdef");

    dafs::BlockInfo b = dafs::CreateBlockInfo(path, identity);

    ASSERT_EQ(path, b.path);
    ASSERT_EQ(identity, b.identity);
}
