#include "time.hpp"

#include <chrono>

uint64_t time_get_current_time()
{
	return std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
}
