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
                      dafs::NetworkSender sender;
                      return dafs::HandleJoinCluster(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestMinusInitiation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleRequestMinusInitiation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestPlusInitiation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleRequestPlusInitiation(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_AcceptPlusInitiation,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleAcceptPlusInitiation(
                          node,
                          message.metadata);
                  }
              },
              {
                  dafs::MessageType::ExitCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleExitCluster(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestPlusExit,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleRequestPlusExit(
                          node,
                          message.metadata,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestMinusExit,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      return dafs::HandleRequestMinusExit(
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
