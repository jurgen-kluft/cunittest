#ifndef __C_CORE_UNITTEST_TIMECONSTRAINT_H__
#define __C_CORE_UNITTEST_TIMECONSTRAINT_H__

#include "cunittest/private/ut_TimeHelpers.h"

namespace UnitTest
{
	class TestResults;

	class TimeConstraint
	{
	public:
							TimeConstraint(int ms, TestResults& result, char const* filename, int lineNumber, char const* testName);
							~TimeConstraint();

	private:
		void				operator=(const TimeConstraint&); 

		TestResults&		mResult;
		int const			mMaxMs;
		char const* const	mFilename;
		int const			mLineNumber;
		char const* const	mTestName;
		Timer				mTimer;
	};

	#define UNITTEST_TIME_CONSTRAINT(ms)		TimeConstraint t__(ms, testResults_ ,__FILE__ ,__LINE__, mTestName)
	#define UNITTEST_TIME_CONSTRAINT_EXEMPT()	mTimeConstraintExempt = true
}

#endif	/// __C_CORE_UNITTEST_TIMECONSTRAINT_H__
