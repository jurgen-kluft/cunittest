#include "xunittest\private\ut_TestList.h"
#include "xunittest\private\ut_Test.h"


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
			int i=0;
			while (inName[i]!='\0' && i<NAME_MAX_LENGTH)
			{
				mName[i] = inName[i];
				i++;
			}
		}
		else
		{
			for (int i=0; i<NAME_MAX_LENGTH; i++)
				mName[i] = ("MainSuite")[i];
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
