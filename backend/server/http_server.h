#pragma once
#include <string>
class HttpServer {
 public:
  HttpServer(int port, std::string frontend_root);
  int run();
 private:
  int port_;
  std::string frontend_root_;
};
