#include "filesystem.hpp"
#include "partition.hpp"


namespace dafs
{
    class Node
    {
    public:

        enum class Slot
        {
            Minus,
            Zero,
            Plus
        };

        Node();

        dafs::Partition& GetPartition(Slot slot);

    private:

        dafs::Partition slot_minus;

        dafs::Partition slot_zero;

        dafs::Partition slot_plus;
    };
}
