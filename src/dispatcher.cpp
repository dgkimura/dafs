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
                      return dafs::HandleReadBlock(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::WriteBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleWriteBlock(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::DeleteBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleDeleteBlock(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::GetNodeDetails,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleGetNodeDetails(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::_JoinCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleJoinCluster(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::_RequestMinusInitiation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleRequestMinusInitiation(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::_RequestPlusInitiation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleRequestPlusInitiation(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::_AcceptInitation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleAcceptInitiation(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::_ConcludeInitation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleConcludeInitation(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::_ConcludeInitation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleConcludeInitation(
                          node,
                          message.metadata);
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
