#ifdef TARGET_N3DS

#include <nn/os.h>

#include "xunittest\private\ut_TimeHelpers.h"

namespace UnitTest
{
	Timer::Timer()
	{
    }

	void Timer::start()
	{
        nn::os::Tick* startTime = reinterpret_cast<nn::os::Tick*>(&mData[0]);
        *startTime = nn::os::Tick::GetSystemCurrent();
        NN_ASSERT(*startTime);
	}

	void Timer::update()
	{
	}

	int Timer::getTimeInMs() const
	{
		nn::os::Tick curTime = nn::os::Tick::GetSystemCurrent();

        const nn::os::Tick* startTime = reinterpret_cast< const nn::os::Tick* >(&mData[0]);

		int elapsedTime = (curTime - *startTime).ToTimeSpan().GetMilliSeconds();
        return elapsedTime;
	}

	void TimeHelpers::sleepMs(int const ms)
	{
		nn::os::Thread::Sleep(nn::fnd::TimeSpan::FromMilliSeconds(ms));
	}

}

#endif
