#pragma once


namespace dafs
{
    enum class ProposalType
    {
        SuperBlockInsert,
        SuperBlockRemove,
        BlockWriteDelta,
    };


    //
    // Hash function for proposal types
    //
    struct ProposalTypeHash
    {
        template<typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };
}
