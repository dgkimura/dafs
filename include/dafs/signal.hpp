#pragma once

#include <condition_variable>
#include <mutex>
#include <string>

#include "dafs/result.hpp"


namespace dafs
{
    class Signal
    {
    public:

        Signal();

        void Set(dafs::Result result);

        dafs::Result Wait();

    private:
        std::mutex mutex;

        std::condition_variable condition;

        bool flag;

        dafs::Result result_;
    };
}
