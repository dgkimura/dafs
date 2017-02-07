#include "dafs/dispatcher.hpp"
#include "dafs/handler.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(dafs::Node& node, dafs::Sender& sender)
        : registered_map {
              {
                  dafs::MessageType::ReadBlock,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleReadBlock(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::WriteBlock,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleWriteBlock(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_Allocate,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleAllocate(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_Allocated,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleAllocated(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestInitiation,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleRequestInitiation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ProcessInitation,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleProcessInitation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ConcludeInitation,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleConcludeInitation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ConcludeInitation,
                  [&node, &sender](dafs::Message message)
                  {
                      dafs::HandleConcludeInitation(
                          node,
                          message.metadata,
                          sender);
                  }
              }
          },
          node(node),
          sender(sender)
    {
    }


    void
    Dispatcher::Process(dafs::Message message)
    {
        dafs::MetaDataParser metadata(message.metadata);
        registered_map[message.type](message);
    }
}
