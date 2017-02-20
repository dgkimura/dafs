#include "dafs/dispatcher.hpp"
#include "dafs/handler.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(dafs::Node& node)
        : registered_map {
              {
                  dafs::MessageType::ReadBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleReadBlock(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::WriteBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleWriteBlock(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::DeleteBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleDeleteBlock(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::GetNodeDetails,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleGetNodeDetails(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestInitiation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleRequestInitiation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ProcessInitation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleProcessInitation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ConcludeInitation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleConcludeInitation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ConcludeInitation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender(message.from);
                      return dafs::HandleConcludeInitation(
                          node,
                          message.metadata,
                          sender);
                  }
              }
          },
          node(node)
    {
    }


    dafs::Message
    Dispatcher::Process(dafs::Message message)
    {
        dafs::MetaDataParser metadata(message.metadata);
        return registered_map[message.type](message);
    }
}
