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

		virtual void*		Allocate(int size) { return 0; }
		virtual void		Deallocate(void* ptr) {}
	};

	static NullAllocator	sNullAllocator;
//	static Allocator*		sAllocator = &sNullAllocator;

	class CountingAllocator : public Allocator
	{
	public:
		CountingAllocator(Allocator* allocator) : mAllocator(allocator)
		{
		}

		virtual void*		Allocate(int size) { mNumAllocations++; return mAllocator->Allocate(size); }
		virtual void		Deallocate(void* ptr) {mNumAllocations--; mAllocator->Deallocate((ptr)); }

		Allocator*	mAllocator;
		int			mNumAllocations;
	};

	static CountingAllocator	sCountingAllocator(&sNullAllocator);

	int				GetNumAllocations()
	{
		return sCountingAllocator.mNumAllocations;
	}

	void			SetAllocator(Allocator* allocator)
	{
		sCountingAllocator.mNumAllocations = 0;
		sCountingAllocator.mAllocator = allocator;
		if (sCountingAllocator.mAllocator == 0)
			sCountingAllocator.mAllocator = &sNullAllocator;
	}

	Allocator*		GetAllocator()
	{
		return &sCountingAllocator;
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
