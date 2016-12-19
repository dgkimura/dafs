#pragma once

#include <string>

#include <paxos/parliament.hpp>

#include "filesystem.hpp"
#include "propose.hpp"
#include "signal.hpp"
#include "serialization.hpp"


namespace dafs
{
    class NodeSet
    {
    public:

        virtual void AddNode(std::string address, short port) = 0;

        virtual void RemoveNode(std::string address, short port) = 0;
    };


    class ReplicatedNodeSet : public NodeSet
    {
    public:

        ReplicatedNodeSet(
            Parliament& parliament,
            dafs::Signal& in_progress);

        ReplicatedNodeSet(
            const ReplicatedNodeSet& other);

        virtual void AddNode(std::string address, short port) override;

        virtual void RemoveNode(std::string address, short port) override;

    private:

        Parliament& parliament;

        dafs::Signal& in_progress;
    };
}
