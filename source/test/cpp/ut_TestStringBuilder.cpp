#include "xunittest\xunittest.h"
#include "xunittest\private\ut_StringBuilder.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestStringBuilder)
{
	UNITTEST_FIXTURE(fixture)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(DefaultIsEmptyString)
		{
			StringBuilder stream;
			CHECK (stream.getText() != 0);
			CHECK_EQUAL ("", stream.getText());
		}

		UNITTEST_TEST(StreamingTextCopiesCharacters)
		{
			StringBuilder stream;
			stream << "Lalala";
			CHECK_EQUAL ("Lalala", stream.getText());
		}

		UNITTEST_TEST(StreamingMultipleTimesConcatenatesResult)
		{
			StringBuilder stream;
			stream << "Bork" << "Foo" << "Bar";
			CHECK_EQUAL ("BorkFooBar", stream.getText());
		}

		UNITTEST_TEST(StreamingIntWritesCorrectCharacters)
		{
			StringBuilder stream;
			stream << 123;
			CHECK_EQUAL ("123", stream.getText());
		}

		UNITTEST_TEST(StreamingFloatWritesCorrectCharacters)
		{
			StringBuilder stream;
			stream << 3.1415f;
			CHECK (gStringFind(stream.getText(), "3.1415"));
		}

		UNITTEST_TEST(StreamingPointerWritesCorrectCharacters)
		{
			StringBuilder stream;
			int* p = (int*)0x1234;
			stream << p;
			CHECK (gStringFind(stream.getText(), "1234"));
		}
		
		UNITTEST_TEST(StreamingSizeTWritesCorrectCharacters)
		{
			StringBuilder stream;
			int s = 53124;
			stream << s;
			CHECK_EQUAL ("53124", stream.getText());
		}
		
		UNITTEST_TEST(StreamInitialCapacityIsCorrect)
		{
			StringBuilder stream(StringBuilder::GROW_CHUNK_SIZE);
			CHECK_EQUAL ((int)StringBuilder::GROW_CHUNK_SIZE, stream.getCapacity());
		}

		UNITTEST_TEST(StreamInitialCapacityIsMultipleOfGrowChunkSize)
		{
			StringBuilder stream(StringBuilder::GROW_CHUNK_SIZE + 1);
			CHECK_EQUAL ((int)StringBuilder::GROW_CHUNK_SIZE * 2, stream.getCapacity());
		}

#ifdef TARGET_PC
		UNITTEST_TEST(ExceedingCapacityGrowsBuffer)
		{
			StringBuilder stream(StringBuilder::STATIC_CHUNK_SIZE);
			for (int i=0; i < ((StringBuilder::STATIC_CHUNK_SIZE-1) / 32); ++i)
				stream << "012345678901234567890123456789xx";
			char const* const oldBuffer = stream.getText();

			stream << "012345678901234567890123456789xxyyyyyyyy";
			CHECK (oldBuffer != stream.getText());
		}

		UNITTEST_TEST(ExceedingCapacityGrowsBufferByGrowChunk)
		{
			StringBuilder stream(StringBuilder::GROW_CHUNK_SIZE);
			stream << "0123456789012345678901234567890123456789";
			CHECK_EQUAL (StringBuilder::GROW_CHUNK_SIZE * 2, stream.getCapacity());
		}
#endif

		UNITTEST_TEST(WritingStringLongerThanCapacityFitsInNewBuffer)
		{
			StringBuilder stream(8);
			stream << "0123456789ABCDEF";
			CHECK_EQUAL ("0123456789ABCDEF", stream.getText());
		}

		UNITTEST_TEST(WritingIntLongerThanCapacityFitsInNewBuffer)
		{
			StringBuilder stream(8);
			stream << "aaaa" << 123456;;
			CHECK_EQUAL ("aaaa123456", stream.getText());
		}

		UNITTEST_TEST(WritingFloatLongerThanCapacityFitsInNewBuffer)
		{
			StringBuilder stream(8);
			stream << "aaaa" << 123456.0f;;
			CHECK_EQUAL ("aaaa123456.000000", stream.getText());
		}
		
		UNITTEST_TEST(WritingSizeTLongerThanCapacityFitsInNewBuffer)
		{
			StringBuilder stream(8);
			stream << "aaaa" << int(32145);
			CHECK_EQUAL ("aaaa32145", stream.getText());
		}
	}
}
UNITTEST_SUITE_END
