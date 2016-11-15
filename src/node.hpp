#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dispatcher.hpp"
#include "filesystem.hpp"
#include "server.hpp"
#include "storage.hpp"


namespace dafs
{
    class Node
    {
    public:

        Node();

        void AddNode(std::string host, short port);

        void RemoveNode(std::string host, short port);
    };
}
