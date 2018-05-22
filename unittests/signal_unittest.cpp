#include <functional>
#include <thread>

#include <gtest/gtest.h>

#include "dafs/signal.hpp"


TEST(SignalTest, testSetResultAndWait)
{
    dafs::Signal s([](){});
    dafs::Result actual, expected{true, "the_contents"};

    std::thread t([&s, &actual](){ actual = s.Wait(); });
    s.Set(expected);
    t.join();

    ASSERT_EQ(expected.success, actual.success);
    ASSERT_EQ(expected.content, actual.content);
}
