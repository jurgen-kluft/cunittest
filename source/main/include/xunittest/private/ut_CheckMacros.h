#ifndef __X_CORE_UNIT_TEST_CHECKMACROS_H__
#define __X_CORE_UNIT_TEST_CHECKMACROS_H__

#include "xunittest\private\ut_Checks.h"
#include "xunittest\private\ut_AssertException.h"
#include "xunittest\private\ut_StringBuilder.h"


#ifdef CHECK
    #error UnitTest redefines CHECK
#endif

#define CHECK(value) \
    try { \
        if (!UnitTest::check(value!=0, true)) \
            testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected 0, but actually "#value" == 0"); \
    } \
    catch (...) { \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
    }

#define CHECK_NULL(value) \
	try { \
		if (value != NULL) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected NULL, but actually "#value" != NULL"); \
	} \
	catch (...) { \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	}

#define CHECK_NOT_NULL(value) \
	try { \
		if (value == NULL) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName,"Expected NOT NULL, but actually " #value" == NULL"); \
	} \
	catch (...) { \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	}

#define CHECK_TRUE(value) \
	try { \
		if (UnitTest::check((value)!=0, false)) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected true but "#value" == false"); \
	} \
	catch (...) { \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	}


#define CHECK_FALSE(value) \
	try { \
		if (UnitTest::check((value)!=0, true)) \
			testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected false but "#value" == true"); \
	} \
	catch (...) { \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK(" #value ")"); \
	}

#define CHECK_EQUAL(expected, actual) \
    try { \
        UnitTest::checkEqual(testResults_, expected, actual, mTestName, __FILE__, __LINE__); \
    } \
    catch (...) { \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
    }

#define CHECK_NOT_EQUAL(expected, actual) \
	try { \
		UnitTest::checkNotEqual(testResults_, expected, actual, mTestName, __FILE__, __LINE__); \
	} \
	catch (...) { \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
	}

#define CHECK_EQUAL_T(expected, actual, msg) \
	try { \
		UnitTest::checkEqual(testResults_, expected, actual, mTestName, __FILE__, __LINE__); \
	} \
	catch (...) { \
		testResults_.onTestFailure(__FILE__, __LINE__, mTestName, msg); \
	}


#define CHECK_CLOSE(expected, actual, tolerance) \
    try { \
        UnitTest::checkClose(testResults_, expected, actual, tolerance, mTestName, __FILE__, __LINE__); \
    } \
    catch (...) { \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")"); \
    }

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance) \
    try { \
        UnitTest::checkArrayClose(testResults_, expected, actual, count, tolerance, mTestName, __FILE__, __LINE__); \
    } \
    catch (...) { \
        testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
    }

#define CHECK_THROW(expression, ExpectedExceptionType) \
    { \
        bool caught_ = false; \
        try { expression; } \
        catch (ExpectedExceptionType const&) { caught_ = true; } \
        if (!caught_) \
            testResults_.onTestFailure(__FILE__, __LINE__, mTestName, "Expected exception: \"" #ExpectedExceptionType "\" not thrown"); \
    }

#define CHECK_ASSERT(expression) \
    CHECK_THROW(expression, UnitTest::AssertException);



#endif	///< __X_CORE_UNIT_TEST_CHECKMACROS_H__