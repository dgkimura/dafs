#pragma once

#include <memory>
#include <string>

#include <paxos/parliament.hpp>

#include "server.hpp"


std::shared_ptr<dafs::Server> CreateServer(std::string dirname);
