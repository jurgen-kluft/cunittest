#ifndef __XUNITTEST_EXCEPTION_H__
#define __XUNITTEST_EXCEPTION_H__

#if defined(TARGET_PC)
	#include "cunittest/private/ut_Exception_Win32.h"
#elif defined(TARGET_MAC)
	#include "cunittest/private/ut_Exception_Mac.h"
#endif

#ifdef _NO_EXCEPTIONS

	#define UT_TRY_BEGIN    {{
	#define UT_CATCH(x)        } if (0) {
	#define UT_CATCH_ALL    } if (0) {
	#define UT_CATCH_END  }}

	#define UT_THROW0()
	#define UT_THROW1(x)
	#define UT_THROW(x, y) x(y)

#else /* _NO_EXCEPTIONS */

	#define UT_TRY_BEGIN    try {
	#define UT_CATCH(x)        } catch (x) {
	#define UT_CATCH_ALL    } catch (...) {
	#define UT_CATCH_END  }

	#define UT_THROW0()      throw ()
	#define UT_THROW1(x)    throw (x)
	#define UT_THROW(x, y) throw x(y)

#endif /* _NO_EXCEPTIONS */



#endif	///< __XUNITTEST_EXCEPTIONH__
