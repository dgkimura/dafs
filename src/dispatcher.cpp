#include "dispatcher.hpp"
#include "handler.hpp"
#include "metadataparser.hpp"
#include "messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(Storage& store_)
        : registered_map {
              { dafs::MessageType::CreateFile, dafs::HandleCreateFile },
              { dafs::MessageType::DeleteFile, dafs::HandleDeleteFile },
              { dafs::MessageType::OpenFile, dafs::HandleOpenFile },
              { dafs::MessageType::ReadFile, dafs::HandleReadFile },
              { dafs::MessageType::WriteFile, dafs::HandleWriteFile },
              { dafs::MessageType::_ReadBlock, dafs::HandleReadBlock },
              { dafs::MessageType::_WriteBlock, dafs::HandleWriteBlock },
              { dafs::MessageType::_Allocate, dafs::HandleAllocate },
              { dafs::MessageType::_Allocated, dafs::HandleAllocated }
          },
          store(store_),
          sender()
    {
    }


    void
    Dispatcher::RouteMessage(dafs::Message message)
    {
        registered_map[message.type]
        (
            store,
            dafs::MetaDataParser(message.metadata),
            sender
        );
    }
}
