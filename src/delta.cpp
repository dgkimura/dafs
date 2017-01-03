#include <sstream>

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
            filename,
            std::string(edit_script).erase(length)
        };
        return delta;
    }


    std::string
    ApplyDelta(Delta& delta, std::string original)
    {
        char new_string[BLOCK_SIZE_IN_BYTES];
        int new_string_length = BLOCK_SIZE_IN_BYTES;

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


    std::string
    ApplyDelta(Delta& delta, std::fstream& stream)
    {
        stream.seekg(0, std::ios::beg);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        std::string original = buffer.str();

        char new_string[BLOCK_SIZE_IN_BYTES];
        int new_string_length = BLOCK_SIZE_IN_BYTES;

        int length = ApplyEditScript(
            &original[0],
            original.length(),
            &delta.difference[0],
            delta.difference.length(),
            new_string,
            new_string_length
        );

        stream.seekp(0, std::ios::beg);
        return std::string(new_string).erase(length);
    }
}
