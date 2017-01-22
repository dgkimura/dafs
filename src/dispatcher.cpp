#include "dafs/dispatcher.hpp"
#include "dafs/handler.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(dafs::Node& node, dafs::Sender& sender)
        : registered_map {
              { dafs::MessageType::CreateFile, dafs::HandleCreateFile },
              { dafs::MessageType::DeleteFile, dafs::HandleDeleteFile },
              { dafs::MessageType::ReadBlock, dafs::HandleReadBlock },
              { dafs::MessageType::WriteBlock, dafs::HandleWriteBlock },
              { dafs::MessageType::_Allocate, dafs::HandleAllocate },
              { dafs::MessageType::_Allocated, dafs::HandleAllocated },
              { dafs::MessageType::_RequestInitiation, dafs::HandleRequestInitiation },
              { dafs::MessageType::_ProcessInitation, dafs::HandleProcessInitation },
              { dafs::MessageType::_ConcludeInitation, dafs::HandleConcludeInitation }
          },
          node(node),
          sender(sender)
    {
    }


    void
    Dispatcher::Process(dafs::Message message)
    {
        dafs::MetaDataParser metadata(message.metadata);
        registered_map[message.type](node, metadata, sender);
    }
}
