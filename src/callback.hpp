#pragma once

#include <functional>
#include <string>


namespace dafs
{
    using ProposalHandler = std::function<void(std::string proposal)>;

    class Callback
    {
    public:

        Callback(ProposalHandler proposal_handler);

        void operator()(std::string proposal);

    private:

        ProposalHandler proposal_handler;
    };
}
