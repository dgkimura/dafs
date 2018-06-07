#include <dafs/sender.hpp>


class MockSender : public dafs::Sender
{
public:

    MockSender()
        : sent_messages()
    {
    }

    ~MockSender()
    {
    }

    virtual void Send(dafs::Message message) override
    {
        sent_messages.push_back(message);
    }

    virtual void Reply(dafs::Message message) override
    {
        sent_messages.push_back(message);
    }

    virtual dafs::Message Receive() override
    {
        dafs::Message m;
        return m;
    }

    std::vector<dafs::Message> sentMessages()
    {
        return sent_messages;
    }

private:

    std::vector<dafs::Message> sent_messages;
};

