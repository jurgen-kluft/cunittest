#ifndef __CUNITTEST_TIMEHELPERS_H__
#define __CUNITTEST_TIMEHELPERS_H__

namespace UnitTest
{
    struct time_t
    {
        unsigned int m_time[2];
    };

    void   g_InitTimer();
    time_t g_TimeStart();
    double g_GetElapsedTimeInMs(time_t start);
    void   g_SleepMs(int ms);

} // namespace UnitTest

#endif ///< __CUNITTEST_TIMEHELPERS_H__
