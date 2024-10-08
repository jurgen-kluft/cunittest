#include "cunittest/cunittest.h"
#include "cunittest/private/ut_StringBuilder.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestStringBuilder)
{
    UNITTEST_FIXTURE(fixture)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(DefaultIsEmptyString)
        {
            StringBuilder stream(FixtureAllocator);
            CHECK(stream.getText() != 0);
            CHECK_EQUAL("", stream.getText());
        }

        UNITTEST_TEST(StreamingTextCopiesCharacters)
        {
            StringBuilder stream(FixtureAllocator);
            stream << "Lalala";
            CHECK_EQUAL("Lalala", stream.getText());
        }

        UNITTEST_TEST(StreamingMultipleTimesConcatenatesResult)
        {
            StringBuilder stream(FixtureAllocator);
            stream << "Bork" << "Foo" << "Bar";
            CHECK_EQUAL("BorkFooBar", stream.getText());
        }

        UNITTEST_TEST(StreamingIntWritesCorrectCharacters)
        {
            StringBuilder stream(FixtureAllocator);
            stream << 123;
            CHECK_EQUAL("123", stream.getText());
        }

        UNITTEST_TEST(StreamingFloatWritesCorrectCharacters)
        {
            StringBuilder stream(FixtureAllocator);
            stream << 3.1415f;
            CHECK(gStringFind(stream.getText(), "3.1415"));
        }

        UNITTEST_TEST(StreamingPointerWritesCorrectCharacters)
        {
            StringBuilder stream(FixtureAllocator);
            int*          p = (int*)0x1234;
            stream << p;
            CHECK(gStringFind(stream.getText(), "1234"));
        }

        UNITTEST_TEST(StreamingSizeTWritesCorrectCharacters)
        {
            StringBuilder stream(FixtureAllocator);
            int           s = 53124;
            stream << s;
            CHECK_EQUAL("53124", stream.getText());
        }

        UNITTEST_TEST(StreamInitialCapacityIsCorrect)
        {
            StringBuilder stream(FixtureAllocator);
            CHECK_EQUAL(480, stream.getCapacity());
        }

        UNITTEST_TEST(WritingStringLongerThanCapacityFitsInNewBuffer)
        {
            StringBuilder stream(FixtureAllocator);
            stream << "0123456789ABCDEF";
            CHECK_EQUAL("0123456789ABCDEF", stream.getText());
        }

        UNITTEST_TEST(WritingIntLongerThanCapacityFitsInNewBuffer)
        {
            StringBuilder stream(FixtureAllocator);
            stream << "aaaa" << 123456;
            CHECK_EQUAL("aaaa123456", stream.getText());
        }

        UNITTEST_TEST(WritingFloatLongerThanCapacityFitsInNewBuffer)
        {
            StringBuilder stream(FixtureAllocator);
            stream << "aaaa" << 123456.0f;
            CHECK_EQUAL("aaaa123456.000000", stream.getText());
        }

        UNITTEST_TEST(WritingSizeTLongerThanCapacityFitsInNewBuffer)
        {
            StringBuilder stream(FixtureAllocator);
            stream << "aaaa" << int(32145);
            CHECK_EQUAL("aaaa32145", stream.getText());
        }
    }
}
UNITTEST_SUITE_END
