#ifndef __XUNITTEST_UTILS_H__
#define __XUNITTEST_UTILS_H__

#include "xunittest\private\ut_Config.h"

namespace UnitTest
{
	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     Copy a string from source to destination limited to a maximum length.
	// Arguments:
	//      dst     - The destination string where you want to string to be copied to.
	//                You should never pass as a <B>NULL</B>.
	//      src     - The source string to copy.
	//      max     - The maximum string length to copy (excluding the '\0' character).
	// Returns:
	//     none
	// Description:
	//      This function is part of the UnitTest library to act independently.
	// See Also:
	//     gStringLength, gStringPrint, gAreStringsEqual
	//------------------------------------------------------------------------------
	extern void	gStringCopy(char* dst, const char* src, int max);

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     Copy a string from source to destination limited to a maximum length.
	// Arguments:
	//      dst     - The destination string where you want to string to be copied to.
	//                You should never pass as a <B>NULL</B>.
	//      src     - The source string to copy.
	//      max     - The maximum string length to copy (excluding the '\0' character).
	// Returns:
	//     none
	// Description:
	//      This function is part of the UnitTest library to act independently.
	// See Also:
	//     gStringCopy, gStringLength, gStringPrint, gAreStringsEqual
	//------------------------------------------------------------------------------
	extern const char*	gStringFind(const char* src, const char* findstr);
	extern char*		gStringFind(char* src, const char* findstr);

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     Return the length of a string
	// Arguments:
	//      src     - The string to compute the length of.
	// Returns:
	//     Returns the number of characters the string holds.
	// Description:
	//      This function is part of the UnitTest library to act independently.
	// See Also:
	//     gStringCopy, gStringPrint, gAreStringsEqual
	//------------------------------------------------------------------------------
	extern int	gStringLength(const char* str);

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     Specialized 'sprintf' like functions
	// Arguments:
	//      dst     - The destination string where you want to formatted string to be written to.
	//                You should never pass as a <B>NULL</B>.
	//      n       - The maximum size of the destination string.
	//      format  - The formatting string to use together with the additional parameter.
	//      @param  - The parameter to format at the %0 position
	// Returns:
	//     none
	// Description:
	//      This function is part of the UnitTest library to act independently.
	// Note:
	//      Platform specific implementations are necessary
	// See Also:
	//     gStringCopy, gStringLength, gAreStringsEqual
	//------------------------------------------------------------------------------
	extern void	gStringPrint(char* dest, int n, const char* format, const void* p);
	extern void	gStringPrint(char* dest, int n, const char* format, char c);
	extern void	gStringPrint(char* dest, int n, const char* format, short s);
	extern void	gStringPrint(char* dest, int n, const char* format, int i);
	extern void	gStringPrint(char* dest, int n, const char* format, long l);
	extern void	gStringPrint(char* dest, int n, const char* format, long long l);
	extern void	gStringPrint(char* dest, int n, const char* format, unsigned char c);
	extern void	gStringPrint(char* dest, int n, const char* format, unsigned short s);
	extern void	gStringPrint(char* dest, int n, const char* format, unsigned int i);
	extern void	gStringPrint(char* dest, int n, const char* format, unsigned long i);
	extern void	gStringPrint(char* dest, int n, const char* format, unsigned long long i);
	extern void	gStringPrint(char* dest, int n, const char* format, float const f);

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     Test if two strings are equal, case sensitive.
	// Arguments:
	//      expected - The string you expect the 'actual' string to be identical to
	//                 You should never pass as a <B>NULL</B>.
	//      actual   - The string you would want to compare to the 'expected' string.
	//                 You should never pass as a <B>NULL</B>.
	// Returns:
	//     none
	// Description:
	//      This function is part of the UnitTest library to act independently.
	// See Also:
	//     gStringCopy, gStringLength, gStringPrint
	//------------------------------------------------------------------------------
	extern bool	gAreStringsEqual(char const* const expected, char const* const actual);

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     Upwards 'power of 2' rounding of an integer
	// Arguments:
	//      n           - The number you want to round to the next power of 2 (e.g. 3 to 4, 20 to 32)
	//      pow2Number  - A power of 2 number (2, 4, 8, 16, 32, 64, 128, 256, ...)
	// Returns:
	//     none
	// Description:
	//      This function is part of the UnitTest library to act independently.
	//------------------------------------------------------------------------------
	inline int	gRoundUpToMultipleOfPow2Number (int n, int pow2Number)
	{
		return (n + (pow2Number - 1)) & ~(pow2Number - 1);
	}

}

#endif	///< __XUNITTEST_UTILS_H__
