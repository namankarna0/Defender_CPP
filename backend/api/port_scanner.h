#pragma once
#include <string>
// Performs a TCP connect scan of ports 1-65535 and returns structured service
// and exposure guidance for ports that accept a connection.
std::string scan_ports_json(const std::string& host);
// Backward-compatible route entry point.
std::string scan_common_ports_json(const std::string& host);
