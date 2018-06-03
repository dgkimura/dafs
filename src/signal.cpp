#include <chrono>
#include <thread>

#include "dafs/signal.hpp"


namespace dafs
{
    Signal::Signal()
        : flag(false)
    {
    }

    void
    Signal::Set(dafs::Result result)
    {
        flag = true;
        result_ = result;
        condition.notify_one();
    }

    dafs::Result
    Signal::Wait()
    {
        std::unique_lock<std::mutex> lock(mutex);

        condition.wait(lock, [&] { return flag; });
        flag = false;
        return result_;
    }
}
