#ifdef TARGET_PSP

#include <kernel.h>
#include <powersvc.h>
#include <assert.h>
#include <stdio.h>

#include "xunittest\private\ut_TimeHelpers.h"

namespace UnitTest
{
    Timer::Timer()
    {
    }

    void Timer::start()
    {
		long long* startTime = reinterpret_cast<long long*>(&mData[2]);
		*startTime= sceKernelGetSystemTimeWide();
        assert(startTime);
    }

    int Timer::getTimeInMs() const
    {
		long long curTime = sceKernelGetSystemTimeWide();
        assert(curTime);

        const long long* startTime = reinterpret_cast<const long long*>(&mData[2]);

        int elapsedTime = curTime - *startTime;
        const float seconds = float(elapsedTime) / 1000.0f;
        return int(seconds);
    }

    void TimeHelpers::sleepMs(int const ms)
    {
		sceKernelDelayThread((SceUInt)ms);
    }
}

#endif