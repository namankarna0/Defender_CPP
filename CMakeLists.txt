cmake_minimum_required(VERSION 3.16)
project(defender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(defender_server
  backend/server/main.cpp
  backend/server/http_server.cpp
  backend/server/router.cpp
  backend/server/static_files.cpp
  backend/api/encryption.cpp
  backend/api/decryption.cpp
  backend/api/password_tools.cpp
  backend/api/system_monitor.cpp
  backend/api/port_scanner.cpp
  backend/encryption-system/encryption-system-text/encrpytion-system-text.cpp
  backend/encryption-system/encryption-system-text/base64.cpp
  backend/passwordTools/random-password-generator/pass.cpp
  backend/passwordTools/attacking-simulator/attack-simu.cpp
  backend/passwordTools/password-strength-and-breach-checker/password.cpp
  backend/utils/json_utils.cpp
  backend/utils/file_utils.cpp
)
target_include_directories(defender_server PRIVATE backend)
target_compile_options(defender_server PRIVATE -Wall -Wextra -Wpedantic)
