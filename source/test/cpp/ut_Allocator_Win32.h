#ifdef TARGET_PC

#include "xunittest/xunittest.h"

class UnitTestAllocator : public UnitTest::Allocator
{
public:
	int		mNumAllocations;

	UnitTestAllocator()
		: mNumAllocations(0)
	{
	}

	void*	Allocate(int size)
	{
		++mNumAllocations;
		return malloc(size);
	}
	void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		free(ptr);
	}

	void	Release()
	{
		if (mNumAllocations != 0)
		{
			UnitTest::Stdout::Trace("ERROR: System Allocator is being released but still has allocations that are not freed\n");
			mNumAllocations = 0;
		}
	}
};

UnitTestAllocator*		gCreateSystemAllocator()
{
	static UnitTestAllocator	sWin32Mem1SystemAllocator;

	return &sWin32Mem1SystemAllocator;
}


#endif
