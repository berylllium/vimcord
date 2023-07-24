#pragma once

#include <json.hpp>

using json = nlohmann::json;

bool json_helper_write_to_file(json to_write, std::string path, bool overwrite = true);
