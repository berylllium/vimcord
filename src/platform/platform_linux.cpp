#ifdef V_ISLINUX

#include "platform/platform.hpp"

#include <iostream>
#include <cstdlib>
#include <format>

void platform_open_default_browser(std::string url)
{
	std::string command = std::format("xdg-open \"{}\"", url);

	std::cout << command << std::endl;

	system(command.c_str());
}

#endif
