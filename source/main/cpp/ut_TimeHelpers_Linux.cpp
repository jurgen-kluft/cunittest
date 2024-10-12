#ifdef TARGET_LINUX

#include "cunittest/private/ut_TimeHelpers.h"

#include <chrono>
#include <unistd.h>

namespace UnitTest
{
	static std::chrono::time_point<std::chrono::system_clock> s_base;

	void g_InitTimer()
	{
        s_base = std::chrono::steady_clock::now();
	}

	time_t g_TimeStart()
    {
        uint64_t start = (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - s_base).count();
        return *(time_t*)(&start);
	}

	double g_GetElapsedTimeInMs(time_t stamp)
	{
		uint64_t const elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - s_base).count() - (uint64_t)stamp;
		return elapsed_us / 1000.0;
	}

	void g_SleepMs(int const ms)
	{
		::sleep(ms * 1000);
	}

}

#endif
