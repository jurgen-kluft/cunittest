#ifndef __XUNITTEST_EXCEPTION_H__
#define __XUNITTEST_EXCEPTION_H__

#if defined(TARGET_PC)
	#include "xunittest\private\ut_Exception_Win32.h"
#elif defined(TARGET_WII)
	#include "xunittest\private\ut_Exception_Wii.h"
#elif defined(TARGET_N3DS)
	#include "xunittest\private\ut_Exception_3ds.h"
#elif defined(TARGET_PS3)
	#ifdef SPU	
		#include "xunittest\private\ut_Exception_Ps3_Spu.h"
		#define _NO_EXCEPTIONS
	#else
		#include "xunittest\private\ut_Exception_Ps3.h"
	#endif

#elif defined(TARGET_PS3_SPU)
	#include "xunittest\private\ut_Exception_Ps3_Spu.h"
	#define _NO_EXCEPTIONS
#elif defined(TARGET_PSP)
	#include "xunittest\private\ut_Exception_psp.h"
#elif defined(TARGET_360)
	#include "xunittest\private\ut_Exception_x360.h"
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
