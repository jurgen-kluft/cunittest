#include "xunittest/private/ut_Config.h"
#include "xunittest/private/ut_Test.h"
#include "xunittest/private/ut_TestList.h"
#include "xunittest/private/ut_TestResults.h"
#include "xunittest/private/ut_TimeHelpers.h"
#include "xunittest/private/ut_AssertException.h"
#include "xunittest/private/ut_StringBuilder.h"
#include "xunittest/private/ut_Stdout.h"


namespace UnitTest
{

	static NullObserver		sNullObserver;
	static Observer*		sObserver = &sNullObserver;

	void			SetObserver(Observer* observer)
	{
		sObserver = observer;
		if (sObserver == 0)
			sObserver = &sNullObserver;
	}

	Observer*		GetObserver()
	{
		return sObserver;
	}
	
	static int				sNumAllocations = 0;

	void			ResetNumAllocations()
	{
		sNumAllocations = 0;
	}
	
	void			IncNumAllocations()
	{
		sNumAllocations++;
	}
	void			DecNumAllocations()
	{
		sNumAllocations--;
	}
	int				GetNumAllocations()
	{
		return sNumAllocations;
	}

	static NullAllocator	sNullAllocator;
	Allocator*				sAllocator = &sNullAllocator;

	void			SetAllocator(Allocator* allocator)
	{
		if (allocator == 0)
			sAllocator = &sNullAllocator;
		else
			sAllocator = allocator;
	}

	namespace __private
	{
		Allocator*		GetAllocator()
		{
			return sAllocator;
		}
	}


	Test::Test(char const* testName, char const* filename, const int lineNumber)
		:mNext(0)
		,mTestName(testName)
		,mFilename(filename)
		,mLineNumber(lineNumber)
		,mTimeConstraintExempt(false)
	{
	}

	Test::~Test()
	{
	}

	void		Test::setup(TestResults& )
	{
	}

	void		Test::teardown(TestResults& )
	{
	}

	void		Test::runImpl(TestResults& ) const
	{
	}

}
