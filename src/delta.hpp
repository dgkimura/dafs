#pragma once

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


    Delta CreateDelta(std::string filename, std::string was, std::string is);

    std::string ApplyDelta(Delta delta, std::string original);
}
