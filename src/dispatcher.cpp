#include "dafs/dispatcher.hpp"
#include "dafs/handler.hpp"
#include "dafs/metadataparser.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    Dispatcher::Dispatcher(dafs::ReplicatedNode& node)
        : registered_map {
              {
                  dafs::MessageType::ReadBlock,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleReadBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::WriteBlock,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleWriteBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::AllocateBlock,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleAllocateBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::DeleteBlock,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleDeleteBlock(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::GetNodeDetails,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleGetNodeDetails(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_JoinCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleJoinCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_RequestJoinCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleRequestJoinCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_AcceptJoinCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleAcceptJoinCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::ExitCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleExitCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_ProposeExitCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleProposeExitCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_PlusExitCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandlePlusExitCluster(
                          node,
                          message,
                          sender);
                  }
              },
              {
                  dafs::MessageType::_MinusExitCluster,
                  [&node](const dafs::Message& message,
                          std::shared_ptr<dafs::Sender> sender)
                  {
                      dafs::HandleMinusExitCluster(
                          node,
                          message,
                          sender);
                  }
              }
          },
          node(node)
    {
    }


    void
    Dispatcher::Process(
        const dafs::Message& message,
        std::shared_ptr<dafs::Sender> sender)
    {
        registered_map[message.type](message, sender);
    }
}
