#include "dafs/identity.hpp"


namespace dafs
{
    bool
    IsLogicallyOrdered(
        dafs::Identity first,
        dafs::Identity second,
        dafs::Identity third)
    {
        return ((first < second) && (second < third)) ||
               ((third < first) && (first < second)) ||
               ((second < third) && (third < first));
    }
}
