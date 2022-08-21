#ifndef __CUNITTEST_TESTLIST_H__
#define __CUNITTEST_TESTLIST_H__

namespace UnitTest
{
	class Test;
	class TestFixture;

	class TestSuite
	{
		enum ESettings
		{
			NAME_MAX_LENGTH = 128
		};

	public:
							TestSuite(const char* inName);

		const char*			getName() const											{ return mName; }
		void				addFixture(TestFixture* inFixture);
		TestFixture*		getFixtures()											{ return mFixtures; }

	private:
		char				mName[NAME_MAX_LENGTH];
		TestFixture*		mFixtures;
		Test*				mHead;
		Test*				mTail;
	};

	class AddFixtureToSuite
	{
	public:
		AddFixtureToSuite(TestSuite* suite, TestFixture* fixture) { suite->addFixture(fixture); }
	};
}

#endif	///< __CUNITTEST_TESTLIST_H__
