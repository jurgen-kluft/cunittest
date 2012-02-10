#ifdef TARGET_PSP

#include "xunittest\private\ut_Utils.h"

#include <stdio.h>
#include <cstdio>

namespace UnitTest
{
    void	gStringPrint(char* dest, int n, const char* format, const void* p)
    {
        sprintf(dest, format, p);
    }

    void	gStringPrint(char* dest, int n, const char* format, char c)
    {
        sprintf(dest, format, c);
    }

    void	gStringPrint(char* dest, int n, const char* format, short s)
    {
        sprintf(dest, format, s);
    }

    void	gStringPrint(char* dest, int n, const char* format, int i)
    {
        sprintf(dest, format, i);
    }

    void	gStringPrint(char* dest, int n, const char* format, long l)
    {
        sprintf(dest, format, l);
    }

    void	gStringPrint(char* dest, int n, const char* format, long long l)
    {
        sprintf(dest, format, l);
    }

    void	gStringPrint(char* dest, int n, const char* format, unsigned char c)
    {
        sprintf(dest, format, c);
    }

    void	gStringPrint(char* dest, int n, const char* format, unsigned short s)
    {
        sprintf(dest, format, s);
    }

    void	gStringPrint(char* dest, int n, const char* format, unsigned int i)
    {
        sprintf(dest, format, i);
    }

    void	gStringPrint(char* dest, int n, const char* format, unsigned long l)
    {
        sprintf(dest, format, l);
    }

    void	gStringPrint(char* dest, int n, const char* format, unsigned long long l)
    {
        sprintf(dest, format, l);
    }

    void	gStringPrint(char* dest, int n, const char* format, float const f)
    {
        sprintf(dest, format, f);
    }

}

#endif
