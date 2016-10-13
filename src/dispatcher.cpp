#include "dispatcher.hpp"
#include "handler.hpp"
#include "messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(Storage store_)
        : store(store_),
          registered_map {
              { dafs::MessageType::CreateFile, dafs::HandleCreateFile },
              { dafs::MessageType::DeleteFile, dafs::HandleDeleteFile },
              { dafs::MessageType::OpenFile, dafs::HandleOpenFile },
              { dafs::MessageType::ReadFile, dafs::HandleReadFile },
              { dafs::MessageType::WriteFile, dafs::HandleWriteFile },
              { dafs::MessageType::Allocate, dafs::HandleAllocate },
              { dafs::MessageType::Allocated, dafs::HandleAllocated }
          }
    {
    }


    void
    Dispatcher::RouteMessage(dafs::Message message)
    {
        registered_map[message.type](store, message.metadata);
    }
}
