#pragma once
#include "utils/request.h"
#include "utils/response.h"
#include <string>
Response serve_static_file(const Request& request, const std::string& frontend_root);
