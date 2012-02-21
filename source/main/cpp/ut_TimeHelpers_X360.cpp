#ifdef TARGET_360

#include "xunittest\private\ut_TimeHelpers.h"

#include <xtl.h>
#include <cassert>

namespace UnitTest
{
    Timer::Timer()
    {
        LARGE_INTEGER* frequency = reinterpret_cast< LARGE_INTEGER* >(&mData);
        BOOL const success = ::QueryPerformanceFrequency(frequency);
        assert(success);
        (void) success;
    }

    void Timer::start()
    {
        LARGE_INTEGER* startTime = reinterpret_cast< LARGE_INTEGER* >(&mData[2]);
        BOOL const success = ::QueryPerformanceCounter(startTime);
        assert(success);
        (void) success;
    }

	void Timer::update()
	{
	}

    int Timer::getTimeInMs() const
    {
        LARGE_INTEGER curTime;
        BOOL const success = ::QueryPerformanceCounter(&curTime);
        assert(success);
        (void) success;

        const LARGE_INTEGER* startTime = reinterpret_cast< const LARGE_INTEGER* >(&mData[2]);

        LARGE_INTEGER elapsedTime;
        elapsedTime.QuadPart = curTime.QuadPart - startTime->QuadPart;

        const LARGE_INTEGER* frequency = reinterpret_cast< const LARGE_INTEGER* >(&mData);
        double const seconds = double(elapsedTime.QuadPart) / (double)frequency->QuadPart;

        return int(seconds * 1000.0f);
    }


    void TimeHelpers::sleepMs(int const ms)
    {
        ::Sleep(ms);
    }

}

#endif
