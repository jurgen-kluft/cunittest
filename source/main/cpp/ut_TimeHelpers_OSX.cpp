#ifdef TARGET_OSX

#include "xunittest/private/ut_TimeHelpers.h"

#include <assert.h>

#ifdef __APPLE__
#define HAVE_MACH_TIMER
#include <mach/mach_time.h>
#endif

#include <unistd.h>

namespace UnitTest
{
	struct the_timer
	{
		uint64_t last; ///< last observation on the counter
		uint64_t rate; ///< counts per second
	};

	Timer::Timer()
	{
		the_timer* t = reinterpret_cast< the_timer* >(&mData[0]);
		mach_timebase_info_data_t rate_nsec;
		mach_timebase_info(&rate_nsec);
		t->rate = 1000000000LL * rate_nsec.numer / rate_nsec.denom;
		t->last = mach_absolute_time();
	}

	void Timer::start()
	{
		the_timer* t = reinterpret_cast< the_timer* >(&mData[0]);
		t->last = mach_absolute_time();
	}

	void Timer::update()
	{
		the_timer* start = reinterpret_cast< the_timer* >(&mData[0]);

		uint64_t const current = mach_absolute_time();
		double const delta = (current - start->last) / (double)start->rate;

		int* timems = reinterpret_cast< int* >(&mData[4]);
		*timems = (int)(delta * 1000.0);
	}

	int Timer::getTimeInMs() const
	{
		int const* timems = reinterpret_cast< int const* >(&mData[4]);
		return *timems;
	}

	void TimeHelpers::sleepMs(int const ms)
	{
		usleep(ms * 1000);
	}

}

#endif
