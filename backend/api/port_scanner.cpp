#include "api/port_scanner.h"
#include "utils/json_utils.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
  struct PortInfo
  {
    int port;
    const char *service;
    const char *function;
    const char *exposure;
  };
  const PortInfo known[] = {
      {21, "FTP", "File Transfer Protocol", "Unencrypted logins or weak credentials can expose files."}, {22, "SSH", "Secure remote administration", "Password guessing or unpatched SSH can expose remote access."}, {23, "Telnet", "Legacy remote terminal", "Credentials are sent in clear text; this service should not be exposed."}, {25, "SMTP", "Mail transfer", "Open relay or outdated mail services can be abused for spam or mail attacks."}, {53, "DNS", "Domain-name resolution", "Misconfiguration can enable amplification or disclose internal records."}, {80, "HTTP", "Web server", "Unpatched web applications can expose data or allow unauthorized actions."}, {110, "POP3", "Mail retrieval", "Plain-text authentication can expose mailbox credentials."}, {111, "RPCbind", "RPC service discovery", "Service enumeration can reveal network services for further attacks."}, {135, "MS RPC", "Windows remote procedure calls", "Exposed RPC services enlarge the Windows attack surface."}, {139, "NetBIOS", "Windows file and printer sharing", "May disclose host and share information to unauthorized users."}, {143, "IMAP", "Mail retrieval", "Weak encryption or credentials can expose mailbox access."}, {161, "SNMP", "Network device monitoring", "Default community strings can disclose device configuration."}, {389, "LDAP", "Directory services", "Anonymous or weakly protected queries can disclose directory data."}, {443, "HTTPS", "Encrypted web server", "Outdated TLS or vulnerable web applications can expose protected data."}, {445, "SMB", "Windows file sharing", "Weak permissions or unpatched SMB can enable data exposure or lateral movement."}, {631, "IPP", "Network printing", "Printer exposure can reveal documents or permit unwanted print jobs."}, {1433, "Microsoft SQL Server", "Database server", "Weak credentials or exposed instances can disclose stored data."}, {3306, "MySQL", "Database server", "Remote access with weak credentials can expose application data."}, {3389, "RDP", "Windows remote desktop", "Credential attacks and unpatched RDP can permit remote access."}, {5432, "PostgreSQL", "Database server", "Weak credentials or broad network access can expose database data."}, {5900, "VNC", "Remote desktop sharing", "Weak authentication can expose interactive desktop access."}, {6379, "Redis", "In-memory data store", "Unauthenticated Redis can expose data and dangerous administration commands."}, {8080, "HTTP alternate", "Web application or proxy", "Administrative web interfaces may be unintentionally exposed."}, {8443, "HTTPS alternate", "Web application or administration", "Administrative web interfaces may be unintentionally exposed."}};
  const PortInfo &describe(int port)
  {
    for (const auto &item : known)
      if (item.port == port)
        return item;
    static const PortInfo unknown{0, "Unknown service", "A service is accepting TCP connections", "Identify the owning application; unnecessary exposed services increase attack surface."};
    return unknown;
  }
  bool is_open(const in_addr &address, int port)
  {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
      return false;
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    sockaddr_in target{};
    target.sin_family = AF_INET;
    target.sin_port = htons(static_cast<uint16_t>(port));
    target.sin_addr = address;
    bool open = connect(fd, reinterpret_cast<sockaddr *>(&target), sizeof target) == 0;
    if (!open && errno == EINPROGRESS)
    {
      fd_set set;
      FD_ZERO(&set);
      FD_SET(fd, &set);
      timeval timeout{0, 150000};
      if (select(fd + 1, nullptr, &set, nullptr, &timeout) > 0)
      {
        int error = 0;
        socklen_t length = sizeof error;
        open = getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &length) == 0 && error == 0;
      }
    }
    close(fd);
    return open;
  }
}

std::string scan_ports_json(const std::string &host)
{
  in_addr address{};
  if (inet_pton(AF_INET, host.c_str(), &address) != 1)
    return json_error("Enter a valid IPv4 address.");
  std::atomic<int> next{1};
  std::mutex mutex;
  std::vector<int> open_ports;
  std::vector<std::thread> workers;
  for (int i = 0; i < 128; ++i)
    workers.emplace_back([&]
                         {
    while (true) {
      const int port = next.fetch_add(1);
      if (port > 65535) break;
      if (is_open(address, port)) { std::lock_guard<std::mutex> lock(mutex); open_ports.push_back(port); }
    } });
  for (auto &worker : workers)
    worker.join();
  std::sort(open_ports.begin(), open_ports.end());
  std::string result = "{\"host\":\"" + json_escape(host) + "\",\"ports_scanned\":65535,\"open_ports\":[";
  for (size_t i = 0; i < open_ports.size(); ++i)
  {
    if (i)
      result += ',';
    const auto &info = describe(open_ports[i]);
    result += "{\"port\":" + std::to_string(open_ports[i]) + ",\"service\":\"" + json_escape(info.service) + "\",\"function\":\"" + json_escape(info.function) + "\",\"exposure\":\"" + json_escape(info.exposure) + "\"}";
  }
  return result + "]}";
}

std::string scan_common_ports_json(const std::string &host)
{
  return scan_ports_json(host);
}
