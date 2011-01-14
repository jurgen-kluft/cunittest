#ifndef __XUNITTEST_TESTMACROS_H__
#define __XUNITTEST_TESTMACROS_H__

#ifdef UNITTEST
    #error UnitTest redefines UNITTEST
#endif

namespace UnitTest																				\
{
	class TestSuite;
	class SuiteNode
	{
	public:
		SuiteNode(SuiteNode*& nodeList, TestSuite* suite) : mNext(0), mTestSuite(suite)
		{
			if (nodeList==0)
				nodeList=this;
			else nodeList->add(this);
		}
		void add(SuiteNode* n) { if (mNext==0) mNext=n; else mNext->add(n); }
		SuiteNode*	mNext;
		TestSuite*	mTestSuite;
	};
}

#define UNITTEST_SUITE_LIST(NameOfList)																\
	namespace UnitTest																				\
	{																								\
		static SuiteNode* NameOfList = 0;															\
	}

#define UNITTEST_SUITE_DECLARE(NameOfList, NameOfSuite)												\
	namespace Suite##NameOfSuite																	\
	{																								\
		extern UnitTest::TestSuite gSuiteObject;			 										\
		UnitTest::SuiteNode gSuiteNode(UnitTest::NameOfList, &gSuiteObject);						\
	}

#define UNITTEST_SUITE_RUN(Reporter, NameOfList)													\
	UnitTest::runAllTests(Reporter, UnitTest::NameOfList);


#define UNITTEST_SUITE_BEGIN(Name)																	\
	namespace Suite##Name																			\
	{																								\
		UnitTest::TestSuite gSuiteObject("Suite_" #Name); 											\
		namespace Suite##Name##Scope

#define UNITTEST_SUITE_END		 }


#define UNITTEST_FIXTURE_EX(Name)																	\
	namespace nsTestFixture##Name																	\
	{																								\
		class TestFixture##Name;																	\
		typedef TestFixture##Name	FixtureInstance;												\
		class TestFixture##Name : public UnitTest::TestFixture										\
		{																							\
		public:																						\
			TestFixture##Name() : UnitTest::TestFixture(#Name, __FILE__, __LINE__) { }				\
		protected:																					\
			virtual void setup(UnitTest::TestResults& testResults_);								\
			virtual void teardown(UnitTest::TestResults& testResults_);								\
		};																							\
		static FixtureInstance		gFixtureInstance;												\

#define UNITTEST_FIXTURE_EX_ADD(Name)																\
		static UnitTest::AddFixtureToSuite gAddFixture(&gSuiteObject, &gFixtureInstance);			\

#define UNITTEST_FIXTURE_EX_OPEN(Name)																\
	}																								\
	namespace nsTestFixture##Name

#define	UNITTEST_FIXTURE_SETUP()																	\
	void FixtureInstance::setup(UnitTest::TestResults& testResults_)

#define	UNITTEST_FIXTURE_TEARDOWN()																	\
	void FixtureInstance::teardown(UnitTest::TestResults& testResults_)

#define UNITTEST_FIXTURE(Name)																		\
	UNITTEST_FIXTURE_EX(Name)																		\
	UNITTEST_FIXTURE_EX_ADD(Name)																	\
	UNITTEST_FIXTURE_EX_OPEN(Name)


#define UNITTEST_FUNC_EX(Name)                                             							\
    class Test##Name : public UnitTest::Test                               							\
    {                                                                      							\
    public:                                                                							\
        Test##Name() : UnitTest::Test(#Name, __FILE__, __LINE__) {}									\
    protected:                                                             							\
        virtual void runImpl(UnitTest::TestResults& testResults_) const;   							\
    } gTest##Name##Instance;                                                						\
    static UnitTest::AddTestToFixture gAddTest##Name(&gFixtureInstance, &gTest##Name##Instance);	\
    void Test##Name::runImpl(UnitTest::TestResults& testResults_) const

#define INTERNAL_UNITTEST_FUNC(Name)			UNITTEST_FUNC_EX(Name)

#define UNITTEST_TEST(Name)						INTERNAL_UNITTEST_FUNC(Name)


#endif	///< __XUNITTEST_TESTMACROS_H__
