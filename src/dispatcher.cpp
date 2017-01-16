#include "dafs/dispatcher.hpp"
#include "dafs/handler.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher()
        : registered_map {
              { dafs::MessageType::CreateFile, dafs::HandleCreateFile },
              { dafs::MessageType::DeleteFile, dafs::HandleDeleteFile },
              { dafs::MessageType::ReadBlock, dafs::HandleReadBlock },
              { dafs::MessageType::WriteBlock, dafs::HandleWriteBlock },
              { dafs::MessageType::_Allocate, dafs::HandleAllocate },
              { dafs::MessageType::_Allocated, dafs::HandleAllocated },
              { dafs::MessageType::_InitiateCluster, dafs::HandleInitiateCluster }
          }
    {
    }


    MessageHandler
    Dispatcher::GetRoutine(dafs::Message message)
    {
        return registered_map[message.type];
    }
}
