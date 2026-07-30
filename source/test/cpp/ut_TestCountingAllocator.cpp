#include "cunittest/cunittest.h"
#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_Test.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_Utils.h"

using namespace UnitTest;

namespace
{
	struct AllocatorRunState
	{
		TestAllocator** mFixtureAllocatorSlot;
		void*           mSetupAllocation;
		void*           mTestAllocation;
	};

	static AllocatorRunState sAllocatorRunState = {nullptr, nullptr, nullptr};

	static TestAllocator* s_GetFixtureAllocator()
	{
		return (sAllocatorRunState.mFixtureAllocatorSlot != nullptr) ? *sAllocatorRunState.mFixtureAllocatorSlot : nullptr;
	}

	struct CollectingReporter : public TestReporter
	{
		enum
		{
			MAX_FAILURES = 8,
			MAX_MESSAGE_LENGTH = 256
		};

		CollectingReporter()
			: mFailureCount(0)
			, mSummaryFailureCount(0)
		{
			for (int i = 0; i < MAX_FAILURES; ++i)
				mFailureMessages[i][0] = '\0';
		}

		virtual void reportTestSuiteStart(int numTests, char const* name)
		{
			(void)numTests;
			(void)name;
		}

		virtual void reportTestSuiteEnd(char const* name, float secondsElapsed)
		{
			(void)name;
			(void)secondsElapsed;
		}

		virtual void reportTestFixtureStart(int numTests, char const* name)
		{
			(void)numTests;
			(void)name;
		}

		virtual void reportTestFixtureEnd(char const* name, float secondsElapsed)
		{
			(void)name;
			(void)secondsElapsed;
		}

		virtual void reportTestStart(char const* name)
		{
			(void)name;
		}

		virtual void reportTestEnd(char const* name, float secondsElapsed)
		{
			(void)name;
			(void)secondsElapsed;
		}

		virtual void reportFailure(char const* file, int const line, char const* name, char const* failure)
		{
			(void)file;
			(void)line;
			(void)name;
			s_RecordMessage(failure);
		}

		virtual void reportFailure(char const* file, int const line, char const* name, char const* failure, const char* message)
		{
			(void)file;
			(void)line;
			(void)name;
			(void)failure;
			s_RecordMessage(message);
		}

		virtual void reportSummary(float secondsElapsed, int failureCount, int testCount)
		{
			(void)secondsElapsed;
			(void)testCount;
			mSummaryFailureCount = failureCount;
		}

		bool Contains(char const* text) const
		{
			for (int i = 0; i < mFailureCount; ++i)
			{
				if (gStringFind(mFailureMessages[i], text) != nullptr)
					return true;
			}
			return false;
		}

		void s_RecordMessage(char const* message)
		{
			if (mFailureCount < MAX_FAILURES)
				gStringCopy(mFailureMessages[mFailureCount], message, MAX_MESSAGE_LENGTH - 1);
			++mFailureCount;
		}

		int  mFailureCount;
		int  mSummaryFailureCount;
		char mFailureMessages[MAX_FAILURES][MAX_MESSAGE_LENGTH];
	};

	static void s_NoOpTest(const char* testName, TestResults& testResults, const float maxTestTimeInMs)
	{
		(void)testName;
		(void)testResults;
		(void)maxTestTimeInMs;
	}

	static void s_LeakInTest(const char* testName, TestResults& testResults, const float maxTestTimeInMs)
	{
		(void)testName;
		(void)testResults;
		(void)maxTestTimeInMs;
		sAllocatorRunState.mTestAllocation = s_GetFixtureAllocator()->Allocate(sizeof(int), sizeof(int));
	}

	static void s_ExtraDeallocateInTest(const char* testName, TestResults& testResults, const float maxTestTimeInMs)
	{
		(void)testName;
		(void)testResults;
		(void)maxTestTimeInMs;
		s_GetFixtureAllocator()->Deallocate(nullptr);
	}

	static void s_LeakInSetup(TestResults& testResults, const char* testName)
	{
		(void)testResults;
		(void)testName;
		sAllocatorRunState.mSetupAllocation = s_GetFixtureAllocator()->Allocate(sizeof(int), sizeof(int));
	}

	static void s_ExtraDeallocateInTeardown(TestResults& testResults, const char* testName)
	{
		(void)testResults;
		(void)testName;
		s_GetFixtureAllocator()->Deallocate(nullptr);
	}

	struct AllocatorRunTree
	{
		MemCheckAllocator mMemcheckAllocator;
		TestContext       mContext;
		TestAllocator*    mFixtureAllocator;
		TestSuite         mSuite;
		TestFixture       mFixture;
		Test              mTest;

		AllocatorRunTree(TestRun run)
			: mFixtureAllocator(nullptr)
			, mSuite("AllocatorSuite", "allocator_suite.cpp")
			, mFixture("AllocatorFixture", "allocator_fixture.cpp", 7, &mFixtureAllocator, &mSuite)
			, mTest("AllocatorTest", "allocator_test.cpp", 13, run, &mFixture)
		{
			mContext.mAllocator = &mMemcheckAllocator;
			mContext.mTestFilter = nullptr;
			sAllocatorRunState.mFixtureAllocatorSlot = &mFixtureAllocator;
			sAllocatorRunState.mSetupAllocation = nullptr;
			sAllocatorRunState.mTestAllocation = nullptr;
		}
	};

	static int s_RunAllocatorFailure(TestRun run, TestReporter& reporter)
	{
		AllocatorRunTree tree(run);
		return TestAllRun(tree.mContext, reporter, &tree.mSuite, 0.0f);
	}
}

UNITTEST_SUITE_BEGIN(TestCountingAllocator)
{
	UNITTEST_FIXTURE(fixture1)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(AllocateIncrementsCount)
		{
			MemCheckAllocator memcheckAllocator;
			TestAllocatorEx   allocator(&memcheckAllocator);

			void* ptr = allocator.Allocate(sizeof(int), sizeof(int));
			CHECK_EQUAL(1, allocator.GetNumAllocations());

			allocator.Deallocate(ptr);
			CHECK_EQUAL(0, allocator.GetNumAllocations());
		}

		UNITTEST_TEST(DeallocateWithoutCorruptionKeepsCorruptionCountZero)
		{
			MemCheckAllocator memcheckAllocator;
			TestAllocatorEx   allocator(&memcheckAllocator);

			void* ptr = allocator.Allocate(sizeof(int), sizeof(int));
			allocator.Deallocate(ptr);

			CHECK_EQUAL(0, allocator.GetNumAllocationCorruptions());
		}

		UNITTEST_TEST(CorruptedAllocationIsReported)
		{
			MemCheckAllocator memcheckAllocator;
			TestAllocatorEx   allocator(&memcheckAllocator);

			char* ptr = (char*)allocator.Allocate(4, sizeof(char));
			ptr[4] = 1;

			int status = 0;
			allocator.Deallocate(ptr, &status);

			CHECK_NOT_EQUAL(0, status);
			CHECK_EQUAL(0, allocator.GetNumAllocations());
			CHECK_EQUAL(1, allocator.GetNumAllocationCorruptions());
		}

		UNITTEST_TEST(ResetExClearsTracking)
		{
			MemCheckAllocator memcheckAllocator;
			TestAllocatorEx   allocator(&memcheckAllocator);

			void* ptr = allocator.Allocate(sizeof(int), sizeof(int));
			allocator.Deallocate(ptr);
			allocator.ResetEx();

			CHECK_EQUAL(0, allocator.GetNumAllocations());
			CHECK_EQUAL(0, allocator.GetNumAllocationCorruptions());
		}

		UNITTEST_TEST(TestRunnerReportsPerTestLeaks)
		{
			CollectingReporter reporter;
			CHECK_EQUAL(1, s_RunAllocatorFailure(s_LeakInTest, reporter));
			CHECK(reporter.mFailureCount > 0);
			CHECK(reporter.Contains("memory leak detected, leaked memory allocations: 1"));
		}

		UNITTEST_TEST(TestRunnerReportsPerTestExtraDeallocations)
		{
			CollectingReporter reporter;
			CHECK_EQUAL(1, s_RunAllocatorFailure(s_ExtraDeallocateInTest, reporter));
			CHECK(reporter.mFailureCount > 0);
			CHECK(reporter.Contains("extra memory deallocations detected, unmatching deallocations: 1"));
		}

		UNITTEST_TEST(TestRunnerReportsSetupTeardownLeaks)
		{
			AllocatorRunTree tree(s_NoOpTest);
			SetSetupForFixture(&tree.mFixture, s_LeakInSetup);
			CollectingReporter reporter;

			CHECK_EQUAL(1, TestAllRun(tree.mContext, reporter, &tree.mSuite, 0.0f));
			CHECK(reporter.mFailureCount > 0);
			CHECK(reporter.Contains("memory leak detected in setup()/teardown(), leaked memory allocations: 1"));
		}

		UNITTEST_TEST(TestRunnerReportsSetupTeardownExtraDeallocations)
		{
			AllocatorRunTree tree(s_NoOpTest);
			SetTeardownForFixture(&tree.mFixture, s_ExtraDeallocateInTeardown);
			CollectingReporter reporter;

			CHECK_EQUAL(1, TestAllRun(tree.mContext, reporter, &tree.mSuite, 0.0f));
			CHECK(reporter.mFailureCount > 0);
			CHECK(reporter.Contains("extra deallocations detected in setup()/teardown(), extra deallocations: 1"));
		}
	}
}
UNITTEST_SUITE_END
