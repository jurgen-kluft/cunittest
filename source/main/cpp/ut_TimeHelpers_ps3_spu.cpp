#ifdef TARGET_PS3_SPU

#include <assert.h>
#include <spu_mfcio_gcc.h>
//#include <spu_printf.h>
#include "xunittest/private/ut_TimeHelpers.h"

namespace UnitTest
{
    Timer::Timer()
    {
    }

    void Timer::start()
    {
		//mData
		//mData[0] : WorkingMode(0: LimitationMode, 1:UpdateMode )
		//LimitationMode:the timer can record up to 53 seconds.
		//UpdateMode: To break this limitation, calling update() function regularly 
		//			  (please ensure the time between 2 update() call is less than 53 seconds)
		//			  (please ensure the time between last update() call and the getTimeInMs() call is less than 53 seconds)
		//mData[2] : start timing
		//mData[4] : previous time(only used in update mode)
		//mData[6] : count the wrapped-rounds(only used in update mode)

		int* updateMode = reinterpret_cast<int*>(&mData[0]);
		*updateMode = 0;

		uint32_t* startTime = reinterpret_cast<uint32_t*>(&mData[2]);
		*startTime = ~spu_read_decrementer();

		uint32_t* prevTime = reinterpret_cast<uint32_t*>(&mData[4]);
		*prevTime = *startTime;

		uint32_t* carryCounter = reinterpret_cast<uint32_t*>(&mData[6]);
		*carryCounter = 0;
		assert(*startTime);
    }

	void Timer::update()
	{
		int* updateMode = reinterpret_cast<int*>(&mData[0]);
		*updateMode = 1;

		uint32_t* prevTime = reinterpret_cast<uint32_t*>(&mData[4]);
		uint32_t currTime = ~spu_read_decrementer();

		//spu_printf("prevTime = %u\n", *prevTime);
		//spu_printf("currTime = %u\n", currTime);
		if ( currTime < *prevTime )
		{
			uint32_t* carryCounter = reinterpret_cast<uint32_t*>(&mData[6]);
			(*carryCounter)++;
			//spu_printf("carryCounter = %u\n", *carryCounter);
		}
		*prevTime = currTime;
	}

    int Timer::getTimeInMs() const
    {

		const int* updateMode = reinterpret_cast<const int*>(&mData[0]);
		if (*updateMode)
		{ 
			uint32_t nowTime = ~spu_read_decrementer();
			const uint32_t* startTime = reinterpret_cast<const uint32_t*>(&mData[2]);
			const uint32_t* carryCounter = reinterpret_cast<const uint32_t*>(&mData[6]);
			uint64_t actualTime = ((uint64_t)nowTime - *startTime + (*carryCounter) * 0xffffffff)/79800;
			//spu_printf("It's update mode\n");
			//spu_printf("actualTime = %d\n", (int)actualTime);
			return (int)actualTime;
		} 
		else
		{
			uint32_t nowTime = ~spu_read_decrementer();
			uint32_t actualTime = 0;
			const uint32_t* startTime = reinterpret_cast<const uint32_t*>(&mData[2]);

			actualTime = (nowTime < *startTime) ?
				(0xffffffff - *startTime + nowTime)/79800: //80mhz
			(nowTime - *startTime)/79800; //80mhz
			return actualTime;
		}

	}

	void TimeHelpers::sleepMs(int const ms)
	{
		uint32_t sleepStartTime = ~spu_read_decrementer();
		//spu_printf("sleep\n");
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