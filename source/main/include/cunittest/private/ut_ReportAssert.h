
#ifndef __CUNITTEST_ASSERT_H__
#define __CUNITTEST_ASSERT_H__

namespace UnitTest
{
    void ReportAssert(char const* description, char const* filename, int lineNumber, const char* message = nullptr);
}

#endif ///< __CUNITTEST_ASSERT_H__
