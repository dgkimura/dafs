#include "topology.hpp"


namespace dafs
{
    Node
    GetPrimary(Topology topology)
    {
        return topology.nodes[TOPOLOGY_SIZE / 2];
    }

    Node
    GetLeft(Topology topology, int index)
    {
        return topology.nodes[(TOPOLOGY_SIZE / 2) - index];
    }

    Node
    GetRight(Topology topology, int index)
    {
        return topology.nodes[(TOPOLOGY_SIZE / 2) + index];
    }
}
