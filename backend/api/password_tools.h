#pragma once
#include <string>
std::string generate_password(int length, bool lower, bool upper, bool numbers, bool symbols);
std::string password_analysis_json(const std::string& password, double attempts_per_second = 1000000000.0);
