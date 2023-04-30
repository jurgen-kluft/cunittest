#ifndef __CUNITTEST_UTILS_H__
#define __CUNITTEST_UTILS_H__

#include "cunittest/private/ut_Config.h"

namespace UnitTest
{
    extern void        gStringCopy(char* dst, const char* src, int max);
    extern const char* gStringFind(const char* src, const char* findstr);
    extern char*       gStringFind(char* src, const char* findstr);
    extern int         gStringLength(const char* str);
    extern void        gStringPrint(char* dest, int n, const char* format, const void* p);
    extern void        gStringPrint(char* dest, int n, const char* format, char c);
    extern void        gStringPrint(char* dest, int n, const char* format, short s);
    extern void        gStringPrint(char* dest, int n, const char* format, int i);
    extern void        gStringPrint(char* dest, int n, const char* format, long l);
    extern void        gStringPrint(char* dest, int n, const char* format, long long l);
    extern void        gStringPrint(char* dest, int n, const char* format, unsigned char c);
    extern void        gStringPrint(char* dest, int n, const char* format, unsigned short s);
    extern void        gStringPrint(char* dest, int n, const char* format, unsigned int i);
    extern void        gStringPrint(char* dest, int n, const char* format, unsigned long i);
    extern void        gStringPrint(char* dest, int n, const char* format, unsigned long long i);
    extern void        gStringPrint(char* dest, int n, const char* format, float const f);
    extern void        gStringPrint(char* dest, int n, const char* format, double const f);
    extern bool        gAreStringsEqual(char const* const expected, char const* const actual);
    inline int         gRoundUpToMultipleOfPow2Number(int n, int pow2Number) { return (n + (pow2Number - 1)) & ~(pow2Number - 1); }
} // namespace UnitTest

#endif ///< __CUNITTEST_UTILS_H__
