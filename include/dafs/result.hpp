#pragma once

#include <string>


namespace dafs
{
    struct Result
    {
        bool success;

        bool complete;

        std::string content;
    };
}
