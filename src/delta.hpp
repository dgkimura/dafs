#ifndef __DELTA_HPP_INCLUDED__
#define __DELTA_HPP_INCLUDED__

#include <string>


namespace dafs
{
    struct Delta
    {
        std::string filename;
        std::string difference;
    };
}

#endif
