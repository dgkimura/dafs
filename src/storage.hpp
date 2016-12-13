#pragma once

#include <string>

#include <paxos/parliament.hpp>

#include "delta.hpp"
#include "disk.hpp"
#include "filesystem.hpp"
#include "propose.hpp"
#include "signal.hpp"
#include "serialization.hpp"


namespace
{
    struct Constant
    {
        static constexpr const char * FileListName = "filelist";

        static constexpr const char * BlockListName = "blocklist";

        static constexpr const char * NodeSetName = "nodeset";

        static constexpr const char * IdentityName = "identity";

        static constexpr const int UnknownId = -1;
    };
};


namespace dafs
{
    class Storage
    {
    public:

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) = 0;

        virtual void Write(BlockInfo info, Delta delta) = 0;
    };


    class ReplicatedStorage : public Storage
    {
    public:

        ReplicatedStorage(
            Parliament& parliament,
            dafs::Signal& in_progress);

        ReplicatedStorage(
            const ReplicatedStorage& other);

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) override;

        virtual void Write(BlockInfo info, Delta delta) override;

    private:

        virtual void do_write(
            BlockInfo info,
            std::string data);

        Parliament parliament;

        Signal& in_progress;
    };
}
