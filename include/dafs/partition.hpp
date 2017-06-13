#pragma once

#include "dafs/blocks.hpp"
#include "dafs/messages.hpp"
#include "dafs/replication.hpp"
#include "dafs/components.hpp"


namespace dafs
{
    class Partition
    {
    public:

        virtual dafs::Identity GetIdentity() = 0;

        virtual dafs::ReplicatedEndpoints GetNodeSetDetails() = 0;

        virtual bool IsAddressResponsive(dafs::Address address) = 0;

        virtual BlockInfo AllocateBlock() = 0;

        virtual void DeleteBlock(BlockInfo block) = 0;

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) = 0;

        virtual BlockIndex GetIndex() = 0;

        virtual void SetMinus(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) = 0;

        virtual void SetZero(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) = 0;

        virtual void SetPlus(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) = 0;

        virtual bool IsActive() = 0;

        virtual bool Acquire() = 0;

        virtual void Release() = 0;
    };


    class EmptyPartition : public Partition
    {
    public:

        EmptyPartition(dafs::Identity identity)
            : identity(identity)
        {
        }

        virtual dafs::Identity GetIdentity() override
        {
            return identity;
        }

        virtual dafs::ReplicatedEndpoints GetNodeSetDetails() override
        {
            return dafs::ReplicatedEndpoints
            {
            };
        }

        virtual bool IsAddressResponsive(dafs::Address address) override
        {
            return true;
        }

        virtual BlockInfo AllocateBlock() override
        {
            dafs::BlockInfo info;
            return info;
        }

        virtual void DeleteBlock(BlockInfo block) override
        {
        }

        virtual BlockFormat ReadBlock(BlockInfo block) override
        {
            return dafs::BlockFormat{""};
        }

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override
        {
        }

        virtual BlockIndex GetIndex() override
        {
            return BlockIndex{};
        }

        virtual void SetMinus(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) override
        {
        }

        virtual void SetZero(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) override
        {
        }

        virtual void SetPlus(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) override
        {
        }


        virtual bool IsActive() override
        {
            return false;
        }

        virtual bool Acquire() override
        {
            return true;
        }

        virtual void Release() override
        {
        }

    private:

        dafs::Identity identity;
    };


    class ReplicatedPartition : public Partition
    {
    public:

        ReplicatedPartition(
            Address address,
            Root root,
            std::chrono::seconds ping_interval
        );

        ReplicatedPartition(
            const ReplicatedPartition& other
        );

        virtual dafs::Identity GetIdentity() override;

        virtual dafs::ReplicatedEndpoints GetNodeSetDetails() override;

        virtual bool IsAddressResponsive(dafs::Address address) override;

        virtual BlockInfo AllocateBlock() override;

        virtual void DeleteBlock(BlockInfo block) override;

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override;

        virtual BlockIndex GetIndex() override;

        virtual void SetMinus(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) override;

        virtual void SetZero(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) override;

        virtual void SetPlus(
            dafs::Address management,
            dafs::Address replication,
            dafs::Identity identity,
            std::string location) override;

        virtual bool IsActive() override;

        virtual bool Acquire() override;

        virtual void Release() override;

    private:

        PaxosReplication replication_;

        ReplicatedStorage store;

        ReplicatedNodeSet nodeset;

        ReplicatedPing ping;

        ReplicatedLock lock;

        dafs::BlockInfo details;
    };
}
