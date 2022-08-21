#ifndef __CUNITTEST_CHECKMACROS_H__
#define __CUNITTEST_CHECKMACROS_H__

#include "cunittest/private/ut_Checks.h"
#include "cunittest/private/ut_AssertException.h"
#include "cunittest/private/ut_StringBuilder.h"


#ifdef CHECK
    #error UnitTest redefines CHECK
#endif

#define CHECK(value) \
    UT_TRY_BEGIN \
        if (!UnitTest::check(value!=0, true)) \
            testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected 0, but actually "#value" == 0"); \
    UT_CATCH_ALL \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
    UT_CATCH_END

#define CHECK_NULL(value) \
	UT_TRY_BEGIN \
		if (value != nullptr) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected nullptr, but actually "#value" != nullptr"); \
    UT_CATCH_ALL \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	UT_CATCH_END

#define CHECK_NOT_NULL(value) \
	UT_TRY_BEGIN \
		if (value == nullptr) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName,"Expected NOT nullptr, but actually " #value" == nullptr"); \
    UT_CATCH_ALL \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	UT_CATCH_END

#define CHECK_TRUE(value) \
	UT_TRY_BEGIN \
		if (UnitTest::check((value)!=0, false)) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected true but "#value" == false"); \
    UT_CATCH_ALL \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	UT_CATCH_END


#define CHECK_FALSE(value) \
	UT_TRY_BEGIN \
		if (UnitTest::check((value)!=0, true)) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected false but "#value" == true"); \
    UT_CATCH_ALL \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	UT_CATCH_END

#define CHECK_EQUAL(expected, actual) \
    UT_TRY_BEGIN \
        UnitTest::checkEqual(testResults_, expected, actual, mTestName, __FILE__, __LINE__); \
    UT_CATCH_ALL \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
    UT_CATCH_END

#define CHECK_NOT_EQUAL(expected, actual) \
	UT_TRY_BEGIN \
		UnitTest::checkNotEqual(testResults_, expected, actual, mTestName, __FILE__, __LINE__); \
    UT_CATCH_ALL \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
	UT_CATCH_END

#define CHECK_EQUAL_T(expected, actual, msg) \
	UT_TRY_BEGIN \
		UnitTest::checkEqual(testResults_, expected, actual, mTestName, __FILE__, __LINE__); \
    UT_CATCH_ALL \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, msg); \
	UT_CATCH_END


#define CHECK_CLOSE(expected, actual, tolerance) \
    UT_TRY_BEGIN \
        UnitTest::checkClose(testResults_, expected, actual, tolerance, mTestName, __FILE__, __LINE__); \
    UT_CATCH_ALL \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")"); \
    UT_CATCH_END

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance) \
    UT_TRY_BEGIN \
        UnitTest::checkArrayClose(testResults_, expected, actual, count, tolerance, mTestName, __FILE__, __LINE__); \
    UT_CATCH_ALL \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
    UT_CATCH_END

#define CHECK_THROW(expression, ExpectedExceptionType) \
    { \
        bool caught_ = false; \
        UT_TRY_BEGIN expression; \
        UT_CATCH(ExpectedExceptionType const&) caught_ = true; \
		UT_CATCH_END \
        if (!caught_) \
            testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected exception: \"" #ExpectedExceptionType "\" not thrown"); \
    }

#define CHECK_ASSERT(expression) \
    CHECK_THROW(expression, UnitTest::AssertException);



#endif	///< __CUNITTEST_CHECKMACROS_H__
