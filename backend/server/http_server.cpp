#include "server/http_server.h"
#include "server/router.h"
#include "utils/request.h"
#include "utils/response.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

namespace
{
    std::string lower(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return value;
    }
    bool receive_request(int fd, Request &request)
    {
        std::string raw;
        char buffer[8192];
        size_t expected = 0;
        while (raw.size() < 1024 * 1024)
        {
            ssize_t read = recv(fd, buffer, sizeof buffer, 0);
            if (read <= 0)
                return false;
            raw.append(buffer, read);
            auto end = raw.find("\r\n\r\n");
            if (end != std::string::npos)
            {
                if (!expected)
                {
                    std::istringstream head(raw.substr(0, end));
                    std::string line;
                    std::getline(head, line);
                    std::istringstream request_line(line);
                    request_line >> request.method >> request.path;
                    while (std::getline(head, line))
                    {
                        if (!line.empty() && line.back() == '\r')
                            line.pop_back();
                        auto colon = line.find(':');
                        if (colon != std::string::npos)
                        {
                            auto key = lower(line.substr(0, colon));
                            auto value = line.substr(colon + 1);
                            while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front())))
                                value.erase(0, 1);
                            request.headers[key] = value;
                        }
                    }
                    expected = end + 4;
                    if (auto it = request.headers.find("content-length"); it != request.headers.end())
                        expected += std::stoul(it->second);
                }
                if (raw.size() >= expected)
                {
                    request.body = raw.substr(end + 4);
                    return true;
                }
            }
        }
        return false;
    }
    std::string status_text(int status)
    {
        switch (status)
        {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 400:
            return "Bad Request";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 413:
            return "Payload Too Large";
        default:
            return "Internal Server Error";
        }
    }
    void send_response(int fd, const Response &response)
    {
        std::ostringstream raw;
        raw << "HTTP/1.1 " << response.status << ' ' << status_text(response.status) << "\r\nContent-Type: " << response.content_type << "\r\nContent-Length: " << response.body.size() << "\r\nConnection: close\r\n";
        for (const auto &[key, value] : response.headers)
            raw << key << ": " << value << "\r\n";
        raw << "\r\n"
            << response.body;
        const auto data = raw.str();
        size_t sent = 0;
        while (sent < data.size())
        {
            auto count = send(fd, data.data() + sent, data.size() - sent, 0);
            if (count <= 0)
                break;
            sent += static_cast<size_t>(count);
        }
    }
}

HttpServer::HttpServer(int port, std::string frontend_root) : port_(port), frontend_root_(std::move(frontend_root)) {}
int HttpServer::run()
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        return 1;
    }
    int yes = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port_);
    if (bind(listener, reinterpret_cast<sockaddr *>(&address), sizeof address) < 0 || listen(listener, 16) < 0)
    {
        perror("bind/listen");
        close(listener);
        return 1;
    }
    std::cout << "DEFENDER++ running at http://localhost:" << port_ << "\n";
    while (true)
    {
        int client = accept(listener, nullptr, nullptr);
        if (client < 0)
            continue;
        Request request;
        if (receive_request(client, request))
            send_response(client, route_request(request, frontend_root_));
        close(client);
    }
}
