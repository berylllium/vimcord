#include "discord_api_handler.hpp"

#include <format>

#include <simple-logger.hpp>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>

#include "http/http_server.hpp"
#include "filesystem.hpp"
#include "platform/platform.hpp"
#include "time.hpp"

static std::filesystem::path AUTH_SESSION_PATH = "./auth_session.json";

static nlohmann::json auth_session;

void discord_api_initialize()
{
	// Initialize the restclient.
	RestClient::init();
}

bool discord_api_does_auth_session_exist()
{
	return std::filesystem::exists(AUTH_SESSION_PATH);
}

bool discord_api_load_auth_session()
{
	if (!discord_api_does_auth_session_exist())
	{
		return false;
	}

	auth_session = filesystem_read_json_from_file(AUTH_SESSION_PATH);

	return true;
}

void discord_api_save_auth_session()
{
	filesystem_write_json_to_file(auth_session, AUTH_SESSION_PATH, true);
}

uint64_t discord_api_auth_session_time_remaining()
{
	uint64_t session_creation_time = auth_session["session_creation_time"].get<uint64_t>();

	uint64_t session_expires_after = auth_session["expires_in"].get<uint64_t>();

	uint64_t current_time = time_get_current_time();

	uint64_t time_expired = current_time - session_creation_time;

	return session_expires_after - time_expired;
}

bool discord_api_is_auth_session_valid()
{
	return discord_api_auth_session_time_remaining() > 0;
}

bool discord_api_request_auth_session(std::string access_code)
{
	RestClient::Connection* conn = new RestClient::Connection("https://discord.com/api/v10");

	RestClient::HeaderFields headers;
	headers["Content-Type"] = "application/x-www-form-urlencoded";
	conn->SetHeaders(headers);

	// TODO: Temporary solution.
	// Get the cliet info.
	nlohmann::json client_info = filesystem_read_json_from_file("./assets/client_info.json");

	std::string query = std::format(
			"client_id={}&client_secret={}&code={}&grant_type=authorization_code&redirect_uri={}",
			client_info["client_id"].get<std::string>(),
			client_info["client_secret"].get<std::string>(),
			access_code,
			client_info["redirect_uri"].get<std::string>()
	);

	RestClient::Response r = conn->post("/oauth2/token", query);

	auth_session = nlohmann::json::parse(r.body);

	discord_api_save_auth_session();

	return true;
}

std::string discord_api_request_access_code()
{
	nlohmann::json config = filesystem_read_json_from_file("./assets/default_config.json");

	std::string url = config["discord_oauth2_url"].get<std::string>();

	sl::log_info("[DiscordApiHandler] Requesting OAuth2 access code using OAuth2 url `{}`...", url);

	platform_open_default_browser(url);

	return http_server_listen_for_access_code();
}
