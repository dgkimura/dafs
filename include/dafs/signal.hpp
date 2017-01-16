#pragma once

#include <condition_variable>
#include <mutex>


namespace dafs
{
    class Signal
    {
    public:

        Signal();

        void Set();

        void Wait();

    private:
        std::mutex mutex;

        std::condition_variable condition;

        bool flag;
    };
}
