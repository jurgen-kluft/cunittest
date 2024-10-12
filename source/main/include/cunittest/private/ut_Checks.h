#ifndef __CUNITTEST_CHECKS_H__
#define __CUNITTEST_CHECKS_H__

#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_StringBuilder.h"

namespace UnitTest
{
	inline bool check(bool value, bool expected)
	{
		return value == expected;
	}

	class TestAllocator;

	template< typename Expected, typename Actual >
	void checkEqual(TestResults& results, Expected const& expected, Actual const& actual, char const* const testName, char const* const filename, int const line, TestAllocator* allocator)
	{
		if (!(expected == actual))
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected ";
			stringBuilder << expected;
			stringBuilder << " but was ";
			stringBuilder << actual;
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}

	template< typename Expected, typename Actual >
	void checkEqualT(TestResults& results, Expected const& expected, Actual const& actual, char const* const testName, char const* const filename, int const line, TestAllocator* allocator, const char* const msg)
	{
		if (!(expected == actual))
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected ";
			stringBuilder << expected;
			stringBuilder << " but was ";
			stringBuilder << actual;
            stringBuilder << ", message: ";
            stringBuilder << msg;
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}

	template< typename Expected, typename Actual >
	void checkNotEqual(TestResults& results, Expected const& expected, Actual const& actual, char const* const testName, char const* const filename, int const line, TestAllocator* allocator)
	{
		if (expected == actual)
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected ";
			stringBuilder << expected;
			stringBuilder << " but was ";
			stringBuilder << actual;
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}

	template< typename Expected, typename Actual >
	void checkNotEqualT(TestResults& results, Expected const& expected, Actual const& actual, char const* const testName, char const* const filename, int const line, TestAllocator* allocator, const char* const msg)
	{
		if (expected == actual)
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected ";
			stringBuilder << expected;
			stringBuilder << " but was ";
			stringBuilder << actual;
			stringBuilder << ", message: ";
			stringBuilder << msg;
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}

	void checkEqual(TestResults& results, char const* expected, char const* actual, char const* const testName, char const* const filename, int const line, TestAllocator* allocator);
	void checkEqual(TestResults& results, char* expected, char* actual, char const* testName, char const* filename, int line, TestAllocator* allocator);
	void checkEqual(TestResults& results, char* expected, char const* actual, char const* testName, char const* filename, int line, TestAllocator* allocator);
	void checkEqual(TestResults& results, char const* expected, char* actual, char const* testName, char const* filename, int line, TestAllocator* allocator);

	template< typename Expected, typename Actual, typename Tolerance >
	bool areClose(Expected const& expected, Actual const& actual, Tolerance const tolerance)
	{
		return (actual >= expected - tolerance) && (actual <= expected + tolerance);
	}

	template< typename Expected, typename Actual, typename Tolerance >
	void checkClose(TestResults& results, Expected const& expected, Actual const& actual, Tolerance const& tolerance, char const* const testName, char const* const filename, int const line, TestAllocator* allocator)
	{
		if (!areClose(expected, actual, tolerance))
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected " << expected << " +/- " << tolerance << " but was " << actual;
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}

	template< typename Expected, typename Actual >
	void checkArrayEqual(TestResults& results, Expected const& expected, Actual const& actual, int const count, char const* const testName, char const* const filename, int const line, TestAllocator* allocator)
	{
		bool equal = true;
		for (int i = 0; i < count; ++i)
			equal &= (expected[i] == actual[i]);

		if (!equal)
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected [ ";
			for (int i = 0; i < count; ++i)
				stringBuilder << expected[i] << " ";
			stringBuilder << "] but was [ ";
			for (int i = 0; i < count; ++i)
				stringBuilder << actual[i] << " ";
			stringBuilder << "]";
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}

	template< typename Expected, typename Actual, typename Tolerance >
	bool checkClose2(Expected const& expected, Actual const& actual, Tolerance const tolerance)
	{
		return ((actual >= expected - tolerance) && (actual <= expected + tolerance));
	}

	template< typename Expected, typename Actual, typename Tolerance >
	void checkArrayClose(TestResults& results, Expected const& expected, Actual const& actual, int const count, Tolerance const& tolerance, char const* const testName, char const* const filename, int const line, TestAllocator* allocator)
	{
		bool equal = true;
		for (int i = 0; i < count; ++i)
			equal &= areClose(expected[i], actual[i], tolerance);

		if (!equal)
		{
			UnitTest::StringBuilder stringBuilder(allocator);
			stringBuilder << "Expected [ ";
			for (int i = 0; i < count; ++i)
				stringBuilder << expected[i] << " ";
			stringBuilder << "] +/- " << tolerance << " but was [ ";
			for (int i = 0; i < count; ++i)
				stringBuilder << actual[i] << " ";
			stringBuilder << "]";
			results.onTestFailure(filename, line, testName, stringBuilder.getText());
		}
	}
}

#endif	///< __CUNITTEST_CHECKS_H__
