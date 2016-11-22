#pragma once

#include <functional>
#include <string>


namespace dafs
{
    template <typename... Ts>
    class Callback
    {

    using Handler = std::function<void(Ts... args)>;

    public:

        Callback(Handler handler=[](Ts... args){})
            : handler(handler)
        {
        }

        void operator()(Ts... args)
        {
            handler(args...);
        }

    private:

        Handler handler;
    };
}
