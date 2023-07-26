#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <simple-logger.hpp>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include <json.hpp>

#include "chat_module_frame.hpp"
#include "discord_api_handler.hpp"
#include "http/http_server.hpp"

using json = nlohmann::json;

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
	MainFrame();
};
 
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
	sl::set_log_to_file(true);
	sl::set_log_time(true);

	sl::log_info("Starting Vimcord...");
	sl::log_info("Initializing wxWidgets App...");
	
	// Initialize the api handler.
	sl::log_info("Initializing the Discord API Handler...");
	discord_api_initialize();

	// Accquire auth session.
	if (discord_api_does_auth_session_exist())
	{
		sl::log_info("Found an existing Auth Session.");
		discord_api_load_auth_session();
	}
	else
	{
		sl::log_info("Existing Auth Session not found. Requesting new Auth Session...");

		std::string access_code = discord_api_request_access_code();

		if (access_code.empty())
		{
			sl::log_fatal("Failed to request OAuth2 access code. Exiting...");
			return false;
		}

		discord_api_request_auth_session(access_code);

		discord_api_save_auth_session();

		sl::log_info("Successfully created a new Auth Session.");
	}

	sl::log_info("Successfully initialized the Discord API Handler.");

	sl::log_info("Successfully initialized the wxWidgets App.");

	MainFrame* frame = new MainFrame();

	frame->Show(true);

	return true;
}

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Vimcord")
{
	ChatModule* chat_module = new ChatModule(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(chat_module, 1, wxEXPAND);

	SetSizerAndFit(sizer);
}
