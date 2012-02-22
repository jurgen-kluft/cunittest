#ifndef __UT_ALLOCATOR_PS3_H__
#define __UT_ALLOCATOR_PS3_H__

#if defined(TARGET_PS3)

#include "xunittest\xunittest.h"
#include "stdlib.h"
#include "stdio.h"

#define u32 unsigned int
#define s32 int
#define u64 long long


class UnitTestAllocator : public UnitTest::Allocator
{
public:
	UnitTestAllocator()
		: mInitialized(0)
		, mDefaultAlignment(4)
		, mAllocationCount(0)
	{
	}

	void			init() 
	{
		mInitialized = 1;
		mDefaultAlignment = 4;
		mAllocationCount = 0;
	}

	bool			isInitialized()
	{
		return mInitialized == 1;
	}

	virtual const char*		name() const
	{
		return "PS3 system allocator";
	}

	virtual void*			Allocate(s32 size)
	{
		++mAllocationCount;
		void* p = memalign(mDefaultAlignment, size);
		return p;
	}

	virtual void*			reallocate(void* ptr, u32 size, u32 alignment)
	{
		void* p = reallocalign(ptr, size, alignment);
		return p;
	}

	virtual void			Deallocate(void* ptr)
	{
		--mAllocationCount;
		free(ptr);
	}

	virtual void			Release()
	{
		if (mAllocationCount != 0) {
			UnitTest::Stdout::Trace("ERROR: System Allocator is being released but still has allocations that are not freed\n");
			mAllocationCount = 0;
		}
		mDefaultAlignment = 4;
		mInitialized = 0;
		mAllocationCount = 0;
	}

protected:
	s32						mInitialized;
	u32						mDefaultAlignment;
	u64						mAllocationCount;
};

UnitTestAllocator*		gCreateSystemAllocator()
{
	static UnitTestAllocator sPs3SystemAllocator;
	if (sPs3SystemAllocator.isInitialized())
		return &sPs3SystemAllocator;
	sPs3SystemAllocator.init();
	return &sPs3SystemAllocator;
}

#endif

#endif /// end of __UT_ALLOCATOR_PS3_H__