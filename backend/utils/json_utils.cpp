#include "utils/json_utils.h"
#include <cctype>

std::string json_escape(const std::string &value)
{
  std::string out;
  for (unsigned char c : value)
  {
    switch (c)
    {
    case '"':
      out += "\\\"";
      break;
    case '\\':
      out += "\\\\";
      break;
    case '\n':
      out += "\\n";
      break;
    case '\r':
      out += "\\r";
      break;
    case '\t':
      out += "\\t";
      break;
    default:
      if (c >= 0x20)
        out += static_cast<char>(c);
    }
  }
  return out;
}

std::unordered_map<std::string, std::string> parse_json_object(const std::string &json)
{
  std::unordered_map<std::string, std::string> values;
  size_t i = 0;
  auto ws = [&]
  { while (i < json.size() && std::isspace(static_cast<unsigned char>(json[i]))) ++i; };
  auto string = [&]()
  { std::string out; if (i >= json.size() || json[i++] != '"') return out; while (i < json.size() && json[i] != '"') { if (json[i] == '\\' && ++i < json.size()) { const char c = json[i++]; out += c == 'n' ? '\n' : c == 't' ? '\t' : c; } else out += json[i++]; } if (i < json.size()) ++i; return out; };
  ws();
  if (i < json.size() && json[i] == '{')
    ++i;
  while (i < json.size())
  {
    ws();
    if (i < json.size() && json[i] == '}')
      break;
    std::string key = string();
    ws();
    if (i >= json.size() || json[i++] != ':')
      break;
    ws();
    std::string value = i < json.size() && json[i] == '"' ? string() : "";
    if (value.empty() && i < json.size() && json[i] != ',')
    {
      size_t start = i;
      while (i < json.size() && json[i] != ',' && json[i] != '}')
        ++i;
      value = json.substr(start, i - start);
      while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())))
        value.pop_back();
    }
    values[key] = value;
    ws();
    if (i < json.size() && json[i] == ',')
      ++i;
  }
  return values;
}

std::string json_error(const std::string &message) { return "{\"error\":\"" + json_escape(message) + "\"}"; }
