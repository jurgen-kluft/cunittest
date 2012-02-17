#ifdef TARGET_PS3_SPU

#include <assert.h>
#include <spu_mfcio_gcc.h>
#include "xunittest\private\ut_TimeHelpers.h"

namespace UnitTest
{
    Timer::Timer()
    {
    }

    void Timer::start()
    {
		uint32_t* startTime = reinterpret_cast<uint32_t*>(&mData[2]);
		*startTime = ~spu_read_decrementer();
		assert(*startTime);
    }

    int Timer::getTimeInMs() const
    {
		uint32_t nowTime = ~spu_read_decrementer();
		uint32_t actualTime = 0;
		const uint32_t* startTime = reinterpret_cast<const uint32_t*>(&mData[2]);

		actualTime = (nowTime < *startTime) ?
			(0xffffffff - *startTime + nowTime)/79800: //80mhz
			(nowTime - *startTime)/79800; //80mhz

		return actualTime;
    }

    void TimeHelpers::sleepMs(int const ms)
    {
		uint32_t sleepStartTime = ~spu_read_decrementer();
		while(1)
		{
			uint32_t nowTime = ~spu_read_decrementer();
			uint32_t elapsedTime = (nowTime < sleepStartTime) ?
				(0xffffffff - sleepStartTime + nowTime)/79800:
			(nowTime - sleepStartTime)/79800;

			if ( elapsedTime >= ms)
			{
				break;
			}
		}
    }

}


#endif