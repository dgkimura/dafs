#include <vector>

#include <dafs/components.hpp>
#include <dafs/messages.hpp>
#include <gtest/gtest.h>

#include "mock_replication.hpp"
#include "mock_sender.hpp"


TEST(ReplicatedPingTest, x)
{
    auto signal = std::make_shared<dafs::Signal>();

    MockReplication replication(
        signal,
        std::vector<dafs::Address>
        {
            dafs::Address("AWAY", 404)
        }
    );

    auto get_endpoints = [](){
        return dafs::ReplicatedEndpoints
        {
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::Address("A", 1)
            },
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::Address("AWAY", 404)
            },
            dafs::Endpoint
            {
                dafs::EmptyAddress(),
                dafs::Address("C", 3)
            }
        };
    };

    auto mock_sender = std::make_shared<MockSender>();
    dafs::ReplicatedPing ping(
        replication,
        dafs::Address("C", 3),
        get_endpoints,
        std::chrono::seconds(0),
        signal);
    ping.SendPing(mock_sender);

    ASSERT_EQ(1, mock_sender->sentMessages().size());
    ASSERT_EQ(
        dafs::MessageType::_ProposeExitCluster,
        mock_sender->sentMessages()[0].type);
}

