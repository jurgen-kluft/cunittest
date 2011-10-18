#include "xunittest\private\ut_Config.h"
#include "xunittest\private\ut_Test.h"
#include "xunittest\private\ut_TestList.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TimeHelpers.h"
#include "xunittest\private\ut_AssertException.h"
#include "xunittest\private\ut_StringBuilder.h"
#include "xunittest\private\ut_Stdout.h"


namespace UnitTest
{
	class NullAllocator : public Allocator
	{
	public:
							NullAllocator() { }

		virtual void*		Allocate(int size, unsigned int alignment) { return 0; }
		virtual void*		Reallocate(void* ptr, int size, unsigned int alignment) { return 0; }
		virtual void		Deallocate(void* ptr) {}
	};

	static NullAllocator	sNullAllocator;

	
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

	Allocator*				sAllocator = &sNullAllocator;

	void			SetAllocator(Allocator* allocator)
	{
		if (allocator == NULL)
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

	class NullObserver : public Observer
	{
	public:
							NullObserver() { }

		virtual void		BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) {}
		virtual void		EndFixture() {}
	};

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
