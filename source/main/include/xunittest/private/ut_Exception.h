#ifndef __XUNITTEST_EXCEPTION_H__
#define __XUNITTEST_EXCEPTION_H__

#if defined(TARGET_PC)
	#include "xunittest\private\ut_Exception_Win32.h"
#elif defined(TARGET_WII)
	#include "xunittest\private\ut_Exception_Wii.h"
#elif defined(TARGET_N3DS)
	#include "xunittest\private\ut_Exception_3ds.h"
#elif defined(TARGET_PS3)
    #include "xunittest\private\ut_Exception_Ps3.h"
#elif defined(TARGET_PS3_SPU)
	#include "xunittest\private\ut_Exception_Ps3_Spu.h"
	#define _NO_EXCEPTIONS
#elif defined(TARGET_PSP)
	#include "xunittest\private\ut_Exception_psp.h"
#elif defined(TARGET_360)
	#include "xunittest\private\ut_Exception_x360.h"
#endif

#ifdef _NO_EXCEPTIONS

	#define _TRY_BEGIN    {{
	#define _CATCH(x)        } if (0) {
	#define _CATCH_ALL    } if (0) {
	#define _CATCH_END  }}

	#define _THROW0()
	#define _THROW1(x)
	#define _THROW(x, y) x(y)

#else /* _NO_EXCEPTIONS */

	#define _TRY_BEGIN    try {
	#define _CATCH(x)        } catch (x) {
	#define _CATCH_ALL    } catch (...) {
	#define _CATCH_END  }

	#define _THROW0()      throw ()
	#define _THROW1(x)    throw (x)
	#define _THROW(x, y) throw x(y)

#endif /* _NO_EXCEPTIONS */



#endif	///< __XUNITTEST_EXCEPTIONH__
