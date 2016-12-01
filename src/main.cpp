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
    store.AddNode("2.2.2.2", 8081);
    store.RemoveNode("2.2.2.2", 8081);

    for (;;);
}
