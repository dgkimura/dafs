#include "diflib.h"

#include "delta.hpp"


namespace dafs
{
    Delta
    DeltaFactory::Create(std::string filename, std::string was, std::string is)
    {
        Delta delta
        {
            filename,
            ""
        };
        return delta;
    }
}
