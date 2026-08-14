#include "server/http_server.h"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[])
{
    int port = 8080;
    if (argc > 1)
    {
        try
        {
            port = std::stoi(argv[1]);
        }
        catch (...)
        {
            std::cerr << "Port must be a number.\n";
            return 1;
        }
    }

    std::filesystem::path root;
    const std::filesystem::path working_root =
        std::filesystem::current_path() / "frontend";
    if (std::filesystem::exists(working_root / "index.html"))
    {
        root = working_root;
    }

    if (root.empty() && argc > 0 && argv[0] != nullptr)
    {
        std::filesystem::path exe = std::filesystem::absolute(std::filesystem::path(argv[0]));
        std::filesystem::path candidate = exe.parent_path();

        for (int i = 0; i < 4; ++i)
        {
            if (std::filesystem::exists(candidate / "frontend") &&
                std::filesystem::exists(candidate / "frontend" / "index.html"))
            {
                root = candidate / "frontend";
                break;
            }
            candidate = candidate.parent_path();
        }
    }

    if (root.empty())
    {
        std::cerr << "Could not locate the frontend directory. Run the server from the project root.\n";
        return 1;
    }

    std::cout << "Serving frontend from: " << root << "\n";
    return HttpServer(port, root.string()).run();
}
