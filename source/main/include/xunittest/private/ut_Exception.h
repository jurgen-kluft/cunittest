#ifndef __X_CORE_UNIT_TEST_EXCEPTION_H__
#define __X_CORE_UNIT_TEST_EXCEPTION_H__

#if defined(TARGET_PC)
	#include "xunittest\private\ut_Exception_Win32.h"
#elif defined(TARGET_WII)
	#include "xunittest\private\ut_Exception_Wii.h"
#elif defined(TARGET_PS3)
    #include "xunittest\private\ut_Exception_Ps3.h"
#elif defined(TARGET_PSP)
	#include "xunittest\private\ut_Exception_psp.h"
#elif defined(TARGET_360)
#include "xunittest\private\ut_Exception_x360.h"
#endif

#endif	///< __X_CORE_UNIT_TEST_EXCEPTIONH__