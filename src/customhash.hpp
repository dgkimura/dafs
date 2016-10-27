#include "filesystem.hpp"


namespace std
{
    template<> struct hash<dafs::BlockFormat>
    {
        std::size_t operator()(dafs::BlockFormat const& s) const
        {
            std::size_t h = 0;
            for (int i=0; i<dafs::BLOCK_SIZE_IN_BYTES; i++)
            {
                h ^= std::hash<char>{}(s.contents[i]);
            }
            return h;
        }
    };
}
