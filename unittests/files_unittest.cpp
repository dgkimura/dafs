#include "gtest/gtest.h"

#include "files.hpp"


TEST(FilesTest, x)
{
    File f;
    ASSERT_EQ(f.Id(), 42);
}
