#pragma once

#include <string>

#include "filesystem.hpp"
#include "persistence.hpp"


namespace dafs
{
    class Storage
    {
    public:

        Storage(std::shared_ptr<Persistence> persister);

        void CreateFile(FileInfo file);

        void DeleteFile(FileInfo file);

        void OpenFile(FileInfo file);

        void ReadFile(FileInfo file);

        void WriteFile(FileInfo info, Bytes data);

        void CreateBlock(BlockInfo block);

        void DeleteBlock(BlockInfo block);

        void ReadBlock(BlockInfo block);

        void WriteBlock(BlockInfo info, Bytes data);

    private:

        std::shared_ptr<Persistence> persister;
    };
}
