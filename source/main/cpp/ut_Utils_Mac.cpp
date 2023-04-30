#ifdef TARGET_MAC

#include "cunittest/private/ut_Utils.h"

#include <stdio.h>
#include <cstdio>

namespace UnitTest
{
	void	gStringPrint(char* dest, int n, const char* format, const void* p)
	{
		snprintf(dest, n, format, p);
	}

	void	gStringPrint(char* dest, int n, const char* format, char c)
	{
		snprintf(dest, n, format, c);
	}

	void	gStringPrint(char* dest, int n, const char* format, short s)
	{
		snprintf(dest, n, format, s);
	}

	void	gStringPrint(char* dest, int n, const char* format, int i)
	{
		snprintf(dest, n, format, i);
	}

	void	gStringPrint(char* dest, int n, const char* format, long l)
	{
		snprintf(dest, n, format, l);
	}

	void	gStringPrint(char* dest, int n, const char* format, long long l)
	{
		snprintf(dest, n, format, l);
	}

	void	gStringPrint(char* dest, int n, const char* format, unsigned char c)
	{
		snprintf(dest, n, format, c);
	}

	void	gStringPrint(char* dest, int n, const char* format, unsigned short s)
	{
		snprintf(dest, n, format, s);
	}

	void	gStringPrint(char* dest, int n, const char* format, unsigned int i)
	{
		snprintf(dest, n, format, i);
	}

	void	gStringPrint(char* dest, int n, const char* format, unsigned long l)
	{
		snprintf(dest, n, format, l);
	}

	void	gStringPrint(char* dest, int n, const char* format, unsigned long long l)
	{
		snprintf(dest, n, format, l);
	}

	void	gStringPrint(char* dest, int n, const char* format, float const f)
	{
		snprintf(dest, n, format, f);
	}

	void	gStringPrint(char* dest, int n, const char* format, double const f)
	{
		snprintf(dest, n, format, f);
	}

}

#endif
