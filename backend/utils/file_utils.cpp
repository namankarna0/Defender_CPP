#include "utils/file_utils.h"
#include <fstream>
#include <filesystem>

bool read_binary_file(const std::string& path, std::string& contents) { std::ifstream in(path, std::ios::binary); if (!in) return false; contents.assign(std::istreambuf_iterator<char>(in), {}); return true; }
std::string mime_type(const std::string& path) { const auto ext = std::filesystem::path(path).extension().string(); if (ext == ".html") return "text/html; charset=utf-8"; if (ext == ".css") return "text/css; charset=utf-8"; if (ext == ".js") return "application/javascript; charset=utf-8"; if (ext == ".json") return "application/json; charset=utf-8"; if (ext == ".svg") return "image/svg+xml"; if (ext == ".png") return "image/png"; if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg"; return "application/octet-stream"; }
std::string safe_filename(const std::string& name) { return std::filesystem::path(name).filename().string(); }
