#include "xunittest/private/ut_TestList.h"
#include "xunittest/private/ut_Test.h"
#include "xunittest/private/ut_Utils.h"


namespace UnitTest
{
	TestSuite::TestSuite(const char* inName)
		:mFixtures(0)
		,mHead(0)
		,mTail(0)
	{
		mName[0] = '\0';

		if (inName!=0)
		{
			gStringCopy(mName, inName, NAME_MAX_LENGTH);
		}
		else
		{
			const char* mainSuiteStr = "MainSuite";
			gStringCopy(mName, mainSuiteStr, NAME_MAX_LENGTH);
		}
	}
	void			TestSuite::addFixture(TestFixture* inFixture)
	{
		if (inFixture==mFixtures)
			return;

		if (mFixtures==0) 
			mFixtures=inFixture;
		else 
			mFixtures->addFixture(inFixture);
	}
}
