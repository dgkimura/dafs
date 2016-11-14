#include "dispatcher.hpp"
#include "filesystem.hpp"
#include "main.hpp"
#include "storage.hpp"


std::shared_ptr<dafs::Server>
CreateServer(std::string dirname)
{
    Parliament p(dirname);
    dafs::ReplicatedStorage store(
        p,
        dafs::CreateBlockInfo(
            "filelist",
            dafs::CreateLocation("localhost")
        ),
        dafs::CreateBlockInfo(
            "blocklist",
            dafs::CreateLocation("localhost")
        )
    );
    dafs::Dispatcher dispatcher(store);
    return std::make_shared<dafs::Server>(dispatcher, "localhost", 8080);
}


int main(void)
{
    auto CreateServer(".");
}
