#ifdef TARGET_PC

#include "cunittest/private/ut_Utils.h"

#include <stdio.h>
#include <cstdio>

namespace UnitTest
{
	int gStringPrint(char* dest, int n, const char* format, const void* p)
	{
		return sprintf_s(dest, n, format, p);
	}

	int gStringPrint(char* dest, int n, const char* format, char c)
	{
		return sprintf_s(dest, n, format, c);
	}

	int gStringPrint(char* dest, int n, const char* format, short s)
	{
		return sprintf_s(dest, n, format, s);
	}

	int gStringPrint(char* dest, int n, const char* format, int i)
	{
		return sprintf_s(dest, n, format, i);
	}

	int gStringPrint(char* dest, int n, const char* format, long l)
	{
		return sprintf_s(dest, n, format, l);
	}

	int gStringPrint(char* dest, int n, const char* format, long long l)
	{
		return sprintf_s(dest, n, format, l);
	}

	int gStringPrint(char* dest, int n, const char* format, unsigned char c)
	{
		return sprintf_s(dest, n, format, c);
	}

	int gStringPrint(char* dest, int n, const char* format, unsigned short s)
	{
		return sprintf_s(dest, n, format, s);
	}

	int gStringPrint(char* dest, int n, const char* format, unsigned int i)
	{
		return sprintf_s(dest, n, format, i);
	}

	int gStringPrint(char* dest, int n, const char* format, unsigned long l)
	{
		return sprintf_s(dest, n, format, l);
	}

	int gStringPrint(char* dest, int n, const char* format, unsigned long long l)
	{
		return sprintf_s(dest, n, format, l);
	}

	int gStringPrint(char* dest, int n, const char* format, float const f)
	{
		return sprintf_s(dest, n, format, f);
	}

	int gStringPrint(char* dest, int n, const char* format, double const f)
	{
		return sprintf_s(dest, n, format, f);
	}
}

#endif
