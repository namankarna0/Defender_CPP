#include "server/static_files.h"
#include "utils/file_utils.h"
#include <filesystem>

Response serve_static_file(const Request& request, const std::string& frontend_root) { Response response; if(request.method!="GET") { response.status=405; response.body="Method not allowed"; response.content_type="text/plain"; return response; } std::string path=request.path; const auto query=path.find('?'); if(query!=std::string::npos) path.erase(query); if(path=="/") path="/index.html"; if(path.find("..")!=std::string::npos) { response.status=404; response.body="Not found"; response.content_type="text/plain"; return response; } const auto file=std::filesystem::path(frontend_root)/(path.substr(1)); if(!read_binary_file(file.string(),response.body)) { response.status=404; response.body="Not found"; response.content_type="text/plain"; return response; } response.content_type=mime_type(file.string()); return response; }
