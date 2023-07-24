#pragma once

#include <json.hpp>

void discord_api_initialize();

bool discord_api_does_auth_session_exist();

bool discord_api_load_auth_session();

void discord_api_save_auth_session();

uint64_t discord_api_auth_session_time_remaining();

bool discord_api_is_auth_session_valid();

bool discord_api_request_auth_session(std::string access_code);

std::string discord_api_request_access_code();
