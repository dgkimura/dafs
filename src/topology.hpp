#include <string>

namespace dafs
{
    //
    // Number (typically primary) which represents total number of nodes any
    // single node will track (including itself).
    //
    const int TOPOLOGY_SIZE = 7;

    struct Node
    {
        int id;
        std::string address;
    };

    struct Topology
    {
        Node nodes[TOPOLOGY_SIZE];
    };

    Node GetPrimary(Topology topology);

    Node GetLeft(Topology topology, int index);

    Node GetRight(Topology topology, int index);
}
