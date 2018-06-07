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
                      dafs::NetworkSender sender;
                      return dafs::HandleReadBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::WriteBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleWriteBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::AllocateBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleAllocateBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::DeleteBlock,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleDeleteBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::GetNodeDetails,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleGetNodeDetails(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_JoinCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleJoinCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestJoinCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleRequestJoinCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_AcceptJoinCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleAcceptJoinCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::ExitCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleExitCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ProposeExitCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleProposeExitCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_PlusExitCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandlePlusExitCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_MinusExitCluster,
                  [&node](dafs::Message message) -> dafs::Message
                  {
                      dafs::NetworkSender sender;
                      return dafs::HandleMinusExitCluster(
                          node,
                          message,
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
        return registered_map[message.type](message);
    }
}
