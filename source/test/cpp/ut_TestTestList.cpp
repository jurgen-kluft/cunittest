#include "xunittest/xunittest.h"
#include "xunittest/private/ut_TestList.h"

using namespace UnitTest;

/*
UNITTEST_SUITE_BEGIN(TestTestList)
{
	UNITTEST_FIXTURE(fixture)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(TestListIsEmptyByDefault)
		{
			TestList list;
			CHECK (list.getHead() == 0);
		}

		UNITTEST_TEST(AddingTestSetsHeadToTest)
		{
			Test test("test");
			TestList list;
			list.add(&test);

			CHECK (list.getHead() == &test);
			CHECK (test.mNext == 0);
		}

		UNITTEST_TEST(AddingSecondTestAddsItToEndOfList)
		{
			Test test1("test1");
			Test test2("test2");

			TestList list;
			list.add(&test1);
			list.add(&test2);

			CHECK (list.getHead() == &test1);
			CHECK (test1.mNext == &test2);
			CHECK (test2.mNext == 0);
		}

		UNITTEST_TEST(ListAdderAddsTestToList)
		{
			TestList list;

			Test test("");    
			ListAdder adder(&list, &test);

			CHECK (list.getHead() == &test);
			CHECK (test.mNext == 0);
		}
	}
}
UNITTEST_SUITE_END
*/
