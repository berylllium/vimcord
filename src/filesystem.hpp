#pragma once

#include <filesystem>
#include <string>

#include <json.hpp>

std::filesystem::path filesystem_get_app_path();
std::filesystem::path filesystem_get_config_path();

bool filesystem_ensure_directory(std::filesystem::path path);

std::string filesystem_read_whole_file(std::filesystem::path path);

nlohmann::json filesystem_read_json_from_file(std::filesystem::path path);

bool filesystem_write_json_to_file(nlohmann::json to_write, std::filesystem::path path, bool overwrite);

