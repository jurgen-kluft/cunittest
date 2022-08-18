#ifndef __XUNITTEST_SUITE_H__
#define __XUNITTEST_SUITE_H__

#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_Utils.h"
#include "cunittest/private/ut_Test.h"
#include "cunittest/private/ut_TestList.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_TestMacros.h"
#include "cunittest/private/ut_CheckMacros.h"
#include "cunittest/private/ut_TestRunner.h"
#include "cunittest/private/ut_TestReporterStdout.h"
#include "cunittest/private/ut_TestReporterTeamCity.h"

#ifdef SPU
void getProgramAndStackSizeForSPU(int* programSize, int* stackSize);
#endif

#endif	///< __XUNITTEST_SUITE_H__
