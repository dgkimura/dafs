#pragma once

#include <functional>
#include <string>


namespace dafs
{
    template <typename Ret, typename... Ts>
    class Callback
    {

    using Handler = std::function<Ret(Ts... args)>;

    public:

        Callback(Handler handler)
            : handler(handler)
        {
        }

        Callback()
        {
        }

        Ret operator()(Ts... args)
        {
            return handler(args...);
        }

    private:

        Handler handler;
    };
}
