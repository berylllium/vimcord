#include "json_helper.hpp"

#include <fstream>

#include "filesystem.hpp"

bool json_helper_write_to_file(json to_write, std::string path, bool overwrite)
{
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
