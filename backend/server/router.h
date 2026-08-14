#pragma once
#include "utils/request.h"
#include "utils/response.h"
Response route_request(const Request& request, const std::string& frontend_root);
