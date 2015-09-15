#ifdef TARGET_WII

#include <revolution/os.h>

#include "xunittest/private/ut_TimeHelpers.h"

namespace UnitTest
{
	Timer::Timer()
	{
        OSTime* frequency = reinterpret_cast<OSTime*>(&mData[0]);
        *frequency = OS_TIMER_CLOCK;
        ASSERT(*frequency);
    }

	void Timer::start()
	{
        OSTime* startTime = reinterpret_cast<OSTime*>(&mData[2]);
        *startTime = OSGetTime();
        ASSERT(*startTime);
	}

	void Timer::update()
	{
	}

	int Timer::getTimeInMs() const
	{
        OSTime curTime = OSGetTime();
        ASSERT(curTime);

        const OSTime* startTime = reinterpret_cast< const OSTime* >(&mData[2]);

        OSTime elapsedTime;
        elapsedTime = curTime - *startTime;

        const OSTime* frequency = reinterpret_cast< const OSTime* >(&mData[0]);
		double de = elapsedTime;
		double df = (*frequency);
        double seconds = de / df;
        return int(seconds * 1000.0f);
	}

	void TimeHelpers::sleepMs(int const ms)
	{
		OSSleepMilliseconds(ms);
	}

}

#endif
