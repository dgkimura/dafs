#include "diflib.h"

#include "delta.hpp"
#include "filesystem.hpp"


namespace dafs
{
    Delta
    CreateDelta(std::string filename, std::string was, std::string is)
    {
        char edit_script[BLOCK_SIZE_IN_BYTES];
        int edit_script_length = BLOCK_SIZE_IN_BYTES;

        int error = ComputeEditScript(
            &was[0],
            was.length(),
            &is[0],
            is.length(),
            edit_script,
            edit_script_length
        );
        Delta delta
        {
            filename,
            std::string(edit_script)
        };
        return delta;
    }


    std::string
    ApplyDelta(Delta delta, std::string original)
    {
        char new_string[BLOCK_SIZE_IN_BYTES];
        int new_string_length = BLOCK_SIZE_IN_BYTES;
        int error = ApplyEditScript(
            &original[0],
            original.length(),
            &delta.difference[0],
            delta.difference.length(),
            new_string,
            new_string_length
        );
        return std::string(new_string);
    }
}