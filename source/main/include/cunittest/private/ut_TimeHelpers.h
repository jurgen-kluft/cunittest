#ifndef __CUNITTEST_TIMEHELPERS_H__
#define __CUNITTEST_TIMEHELPERS_H__

namespace UnitTest
{
    void   g_InitTimer();

    unsigned int g_TimeStart();
    double g_GetElapsedTimeInMs(unsigned int start);
    void   g_SleepMs(int ms);

} // namespace UnitTest

#endif ///< __CUNITTEST_TIMEHELPERS_H__
