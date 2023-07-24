#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

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
	// Initialize the api handler.
	discord_api_initialize();

	// Accquire auth session.
	if (discord_api_does_auth_session_exist())
	{
		discord_api_load_auth_session();
	}
	else
	{
		discord_api_request_auth_session(discord_api_request_access_code());
		discord_api_save_auth_session();
	}

	std::cout << "Loaded." << std::endl;

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
