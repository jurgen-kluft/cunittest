#ifndef __XUNITTEST_SUITE_H__
#define __XUNITTEST_SUITE_H__

#include "xunittest/private/ut_Config.h"
#include "xunittest/private/ut_Utils.h"
#include "xunittest/private/ut_Test.h"
#include "xunittest/private/ut_TestList.h"
#include "xunittest/private/ut_TestResults.h"
#include "xunittest/private/ut_TestMacros.h"
#include "xunittest/private/ut_CheckMacros.h"
#include "xunittest/private/ut_TestRunner.h"
#include "xunittest/private/ut_TestReporterStdout.h"
#include "xunittest/private/ut_TestReporterTeamCity.h"

#ifdef SPU
void getProgramAndStackSizeForSPU(int* programSize, int* stackSize);
#endif

#endif	///< __XUNITTEST_SUITE_H__
