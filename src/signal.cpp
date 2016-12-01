#include "signal.hpp"


namespace dafs
{
    Signal::Signal()
    {
        flag = false;
    }

    void
    Signal::Set()
    {
        flag = true;
        condition.notify_one();
    }

    void
    Signal::Wait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [&] { return flag; });
    }
}
