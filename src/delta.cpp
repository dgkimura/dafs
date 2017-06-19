#include <sstream>

#include <diflib.h>

#include "dafs/blocks.hpp"
#include "dafs/delta.hpp"


namespace dafs
{
    Delta
    CreateDelta(std::string was, std::string is)
    {
        char edit_script[BLOCK_SIZE_IN_BYTES * 2];
        int edit_script_length = BLOCK_SIZE_IN_BYTES * 2;

        int length = ComputeEditScript(
            &was[0],
            was.length(),
            &is[0],
            is.length(),
            edit_script,
            edit_script_length
        );
        Delta delta
        {
            std::string(edit_script).erase(length)
        };
        return delta;
    }


    std::string
    ApplyDelta(Delta& delta, std::iostream& stream)
    {
        stream.seekg(0, std::ios::beg);
        std::stringstream buffer;
        buffer << stream.rdbuf();

        std::string original = buffer.str();
        char new_string[BLOCK_SIZE_IN_BYTES * 2];
        int new_string_length = BLOCK_SIZE_IN_BYTES * 2;

        int length = ApplyEditScript(
            &original[0],
            original.length(),
            &delta.difference[0],
            delta.difference.length(),
            new_string,
            new_string_length
        );
        return std::string(new_string).erase(length);
    }
}
