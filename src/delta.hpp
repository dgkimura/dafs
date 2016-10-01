#ifndef __DELTA_HPP_INCLUDED__
#define __DELTA_HPP_INCLUDED__

#include <string>


namespace dafs
{

    //
    // Edits to files are represented by delta objects. It contains all the
    // necessary data to update the file from one version to the next.
    //
    struct Delta
    {
        std::string filename;
        std::string difference;
    };


    class DeltaFactory
    {
    public:

        static Delta Create(std::string filename, std::string was, std::string is);
    };
}

#endif
