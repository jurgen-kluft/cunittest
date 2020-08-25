#ifndef __XUNITTEST_TEST_H__
#define __XUNITTEST_TEST_H__

namespace UnitTest
{
	class TestResults;

	class Test
	{
	public:
							Test(char const* testName, char const* filename = "", const int lineNumber = 0);
		virtual				~Test();

		virtual void		run(TestResults& testResults, int const maxTestTimeInMs);

		inline void			addTest(Test* inTest)	{ if (mNext!=0) mNext->addTest(inTest); else mNext=inTest; }

		Test*				mNext;
		char const* const	mTestName;
		char const* const	mFilename;
		int const			mLineNumber;
		mutable bool		mTimeConstraintExempt;

	protected:
		virtual void		setup(TestResults& testResults_);
		virtual void		teardown(TestResults& testResults_);

		virtual void		runImpl(TestResults& testResults_) const;

		// Copy Constructor & Assignment operator are private
							Test(Test const&);
		Test&				operator =(Test const&);
	};


	class TestFixture : public Test
	{
	public:
							TestFixture(char const* testName, char const* filename = "", int lineNumber = 0)
								: Test(testName, filename, lineNumber)
								, mStep(FIXTURE_SETUP)
								, mTests(0)
								, mNextFixture(0) {}

		virtual void		run(TestResults& testResults, int const maxTestTimeInMs);

		void				addFixtureTest(Test* inTest)						{ if (mTests!=0) mTests->addTest(inTest); else mTests=inTest; }
		void				addFixture(TestFixture* fixture)					{ if (mNextFixture!=0) mNextFixture->addFixture(fixture); else mNextFixture=fixture; }
		TestFixture*		getNext()											{ return mNextFixture; }

	protected:
		enum EStep
		{
			FIXTURE_SETUP,
			FIXTURE_UNITTESTS,
			FIXTURE_TEARDOWN,
		};
		EStep				mStep;
		Test*				mTests;
		TestFixture*		mNextFixture;
	};

	class AddTestToFixture
	{
	public:
		AddTestToFixture(TestFixture* fixture, Test* test) { fixture->addFixtureTest(test); }
	};

}

#endif	///< __XUNITTEST_TEST_H__
