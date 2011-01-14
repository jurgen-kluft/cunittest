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
#elif defined(TARGET_PSP)
	#include "xunittest\private\ut_Exception_psp.h"
#elif defined(TARGET_360)
	#include "xunittest\private\ut_Exception_x360.h"
#endif

#endif	///< __XUNITTEST_EXCEPTIONH__
