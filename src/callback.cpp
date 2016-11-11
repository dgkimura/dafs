#include "callback.hpp"


namespace dafs
{
    Callback::Callback(ProposalHandler proposal_handler)
        : proposal_handler(proposal_handler)
    {
    }


    void
    Callback::operator()(std::string proposal)
    {
        proposal_handler(proposal);
    }
}
