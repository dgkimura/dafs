#include "gtest/gtest.h"

#include "dafs/callback.hpp"


TEST(CallbackTest, testCallbackIsCalled)
{
    bool was_called = false;
    dafs::Callback<void, int> c([&](int i){ was_called = true; });

    c(42);
    ASSERT_TRUE(was_called);
}

