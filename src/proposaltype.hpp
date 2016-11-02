#pragma once


namespace dafs
{
    enum class ProposalType
    {
        CreateFile,
        RemoveFile,
        CreateBlock,
        RemoveBlock,
        WriteDelta
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
