#include "filesystem.hpp"

#include <fstream>
#include <sstream>

static std::filesystem::path APP_DIR_PATH = "~/.vimcord/";
static std::filesystem::path CONFIG_DIR_PATH = "~/.config/vimcord/";

static std::filesystem::path DISCORD_API_AUTH_SESSION_PATH = APP_DIR_PATH / "auth/discord_api_auth_session.json";

std::filesystem::path filesystem_get_app_path()
{
	return APP_DIR_PATH;
}

std::filesystem::path filesystem_get_config_path()
{
	return CONFIG_DIR_PATH;
}

bool filesystem_ensure_directory(std::filesystem::path path)
{
	std::filesystem::create_directories(path);

	return true;
}

std::string filesystem_read_whole_file(std::filesystem::path path)
{
	std::ifstream file(path);

	std::stringstream ss;
	
	ss << file.rdbuf();

	return ss.str();
}

nlohmann::json filesystem_read_json_from_file(std::filesystem::path path)
{
	return nlohmann::json::parse(filesystem_read_whole_file(path));
}

bool filesystem_write_json_to_file(nlohmann::json to_write, std::filesystem::path path, bool overwrite)
{
	filesystem_ensure_directory(path.parent_path());

	std::ios_base::openmode mode = std::ios::out;

	if (overwrite)
	{
		mode |= std::ios::trunc;
	}

	std::ofstream file(path, mode);

	file << to_write.dump();

	file.close();

	return true;
}
