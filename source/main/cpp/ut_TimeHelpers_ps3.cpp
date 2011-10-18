#ifdef TARGET_PS3

#include <assert.h>
#include <sys/timer.h>
#include <sys/sys_time.h>

#include "xunittest\private\ut_TimeHelpers.h"

namespace UnitTest
{
    Timer::Timer()
    {
        uint64_t* frequency = reinterpret_cast<uint64_t*>(&mData);
        *frequency = sys_time_get_timebase_frequency();
        assert(*frequency);
    }

    void Timer::start()
    {
        system_time_t* startTime = reinterpret_cast<system_time_t*>(&mData[2]);
        *startTime = sys_time_get_system_time();
        assert(*startTime);
    }

    int Timer::getTimeInMs() const
    {
        system_time_t curTime = sys_time_get_system_time();
        assert(curTime);

        const system_time_t* startTime = reinterpret_cast< const system_time_t* >(&mData[2]);

        system_time_t elapsedTime;
        elapsedTime = curTime - *startTime;

        const system_time_t* frequency = reinterpret_cast< const system_time_t* >(&mData);
//		double const  seconds   = double(elapsedTime) / (double)(*frequency);
		double const  seconds   = double(elapsedTime) / 1000.0;

        return int(seconds);
    }

    void TimeHelpers::sleepMs(int const ms)
    {
        sys_timer_sleep((second_t)ms / 1000);
    }

}

#endif
