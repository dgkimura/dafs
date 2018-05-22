#include <chrono>
#include <thread>

#include "dafs/signal.hpp"


namespace dafs
{
    Signal::Signal(std::function<void(void)> retry)
        : flag(false),
          retry(retry)
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
        while (condition.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout)
        {
            if (result_.complete)
            {
                flag = false;
                return result_;
            }
            else
            {
                retry();
            }

        }
        return dafs::Result{};
    }
}
