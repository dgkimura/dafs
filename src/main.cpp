#include <boost/filesystem.hpp>

#include "main.hpp"
#include "storage.hpp"


int main(void)
{
    if (!boost::filesystem::exists("node-1"))
    {
        boost::filesystem::create_directory("node-1");
    }


    auto store = dafs::ReplicatedStorage("node-1", "127.0.0.1:8080", 42);
    store.AddNode("127.0.0.1", 8081);

    for (;;);
}
