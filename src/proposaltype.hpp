#pragma once


namespace dafs
{
    enum class ProposalType
    {
        SuperBlockInsert,
        SuperBlockRemove,
        BlockWriteDelta,
    };
}
