#include <cstring>

#include "dafs/blocks.hpp"


namespace dafs
{
    BlockIndex
    SplitUpperIndex(
        BlockIndex index,
        dafs::Identity lower,
        dafs::Identity divider,
        dafs::Identity upper)
    {
        BlockIndex indexes;

        if (divider < upper)
        {
            // [... lower - divider - upper ...]
            for (auto block : index.items)
            {
                if (divider <= block.identity && block.identity < upper)
                {
                    indexes.items.push_back(block);
                }
            }
        }
        else if (upper < lower && lower < divider)
        {
            // [- upper ... lower - divodor -]
            for (auto block : index.items)
            {
                if (divider <= block.identity || block.identity < upper)
                {
                    indexes.items.push_back(block);
                }
            }
        }

        return indexes;
    }


    BlockIndex
    SplitLowerIndex(
        BlockIndex index,
        dafs::Identity lower,
        dafs::Identity divider,
        dafs::Identity upper)
    {
        BlockIndex indexes;

        if (lower < divider)
        {
            // [... lower - divider - upper ...]
            for (auto block : index.items)
            {
                if (lower <= block.identity && block.identity < divider)
                {
                    indexes.items.push_back(block);
                }
            }
        }
        else if (divider < upper && upper < lower)
        {
            // [- divider - upper ... lower -]
            for (auto block : index.items)
            {
                if (lower <= block.identity || block.identity < divider)
                {
                    indexes.items.push_back(block);
                }
            }
        }

        return indexes;
    }
}
