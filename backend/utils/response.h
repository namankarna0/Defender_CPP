#pragma once
#include <string>
#include <unordered_map>

struct Response {
  int status = 200;
  std::string content_type = "application/json; charset=utf-8";
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};
