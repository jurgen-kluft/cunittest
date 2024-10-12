#ifdef TARGET_LINUX

#include "cunittest/private/ut_TimeHelpers.h"

#include <chrono>
#include <unistd.h>

namespace UnitTest
{
	static uint64_t s_numer;
	static uint64_t s_denom;
	static uint64_t s_base;

	void g_InitTimer()
	{
        s_base = std::chrono::steady_clock::now();
	}

	time_t g_TimeStart()
    {
        uint64_t start = (uint64_t)std::chrono::steady_clock::now() - s_base;
        return *(time_t*)(&start);
	}

	double g_GetElapsedTimeInMs(time_t stamp)
	{
		uint64_t const last = s_base + *(uint64_t*)(&stamp);
		uint64_t const current = std::chrono::steady_clock::now();
		std::chrono::duration_cast<std::chrono::milliseconds>(current - last).count()
		return ms;
	}

	void g_SleepMs(int const ms)
	{
		::sleep(ms * 1000);
	}

}

#endif
