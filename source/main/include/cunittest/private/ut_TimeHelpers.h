#ifndef __CUNITTEST_TIMEHELPERS_H__
#define __CUNITTEST_TIMEHELPERS_H__

#include "cunittest/private/ut_Types.h"

namespace UnitTest
{
    typedef u64 time_t;

    void   g_InitTimer();
    time_t g_TimeStart();
    double g_GetElapsedTimeInMs(time_t start);
    void   g_SleepMs(int ms);

} // namespace UnitTest

#endif ///< __CUNITTEST_TIMEHELPERS_H__
