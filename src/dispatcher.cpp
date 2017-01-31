#include "dafs/dispatcher.hpp"
#include "dafs/handler.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(dafs::Node& node, dafs::Sender& sender)
        : registered_map {
              { dafs::MessageType::ReadBlock,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleReadBlock(node, metadata, sender); } },
              { dafs::MessageType::WriteBlock,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleWriteBlock(node, metadata, sender); } },
              { dafs::MessageType::_Allocate,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleAllocate(node, metadata, sender); } },
              { dafs::MessageType::_Allocated,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleAllocated(node, metadata, sender); } },
              { dafs::MessageType::_RequestInitiation,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleRequestInitiation(node, metadata, sender); } },
              { dafs::MessageType::_ProcessInitation,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleProcessInitation(node, metadata, sender); } },
              { dafs::MessageType::_ConcludeInitation,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleConcludeInitation(node, metadata, sender); } },
              { dafs::MessageType::_ConcludeInitation,
                    [&node, &sender](dafs::MetaDataParser metadata)
                        { dafs::HandleConcludeInitation(node, metadata, sender); } }
          },
          node(node),
          sender(sender)
    {
    }


    void
    Dispatcher::Process(dafs::Message message)
    {
        dafs::MetaDataParser metadata(message.metadata);
        registered_map[message.type](metadata);
    }
}
