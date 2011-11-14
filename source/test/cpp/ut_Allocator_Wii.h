#if defined(TARGET_WII)

#include "xunittest\xunittest.h"
#include <revolution\mem.h>
#include <revolution\os.h>
#include <string.h>

#define u32 unsigned int
#define s32 int
#define u64 long long

inline s32        alignUp(s32 integer, s32 alignment)
{
	return ((integer + (alignment-1)) & (~(alignment-1)));
}

class UnitTestAllocator : public UnitTest::Allocator
{
	const char*				mName;
	s32						mDefaultAlignment;
	MEMHeapHandle			mExpHeapHandle;
	MEMAllocator			mMemAllocator;
	void*					mArenaLo;
	void*					mArenaHi;
	u64						mAllocationCount;

public:
	UnitTestAllocator() 
		: mName("Wii Allocator")
		, mDefaultAlignment(4)
		, mArenaLo(NULL)
		, mArenaHi(NULL)
		, mAllocationCount(0)
	{
	}

	virtual const char*	name() const
	{
		return mName;
	}

	bool				isInitialized() const
	{
		return mArenaLo!=NULL || mArenaHi!=NULL;
	}

	bool				initMem1()
	{
		if (isInitialized())
			return true;

		// Heap on MEM1
		mArenaLo = OSGetMEM1ArenaLo();
		mArenaHi = OSGetMEM1ArenaHi();

		mExpHeapHandle = MEMCreateExpHeap(mArenaLo, (u32)mArenaHi - (u32)mArenaLo);
		if ( mExpHeapHandle == MEM_HEAP_INVALID_HANDLE )
		{
			// MEM1 heap allocation error
			mArenaLo = NULL;
			mArenaHi = NULL;
			return false;
		}
		OSSetMEM1ArenaLo(mArenaHi);
		MEMInitAllocatorForExpHeap(&mMemAllocator, mExpHeapHandle, mDefaultAlignment);

		return true;
	}

	bool				initMem2()
	{
		if (isInitialized())
			return true;

		// Heap on MEM2
		mArenaLo = OSGetMEM2ArenaLo();
		mArenaHi = OSGetMEM2ArenaHi();

		mExpHeapHandle = MEMCreateExpHeap(mArenaLo, (u32)mArenaHi - (u32)mArenaLo);
		if ( mExpHeapHandle == MEM_HEAP_INVALID_HANDLE )
		{
			// MEM2 heap allocation error
			mArenaLo = NULL;
			mArenaHi = NULL;
			return false;
		}
		OSSetMEM2ArenaLo(mArenaHi);
		MEMInitAllocatorForExpHeap(&mMemAllocator, mExpHeapHandle, mDefaultAlignment);

		return true;
	}

	struct header
	{
		void*	real_ptr;
		u32		real_size;
	};

	u32					recalc_size(s32 size, s32 alignment)
	{
		alignment = alignment > mDefaultAlignment ? alignment : mDefaultAlignment;
//		alignment = max(alignment, mDefaultAlignment);
		return size + sizeof(header) + alignment*2;
	}

	static header*		get_header(void* ptr)
	{
		header* _header = (header*)((u32)ptr - sizeof(header));
		return _header;
	}

	static void*		set_header(void* ptr, s32 size, s32 requested_size, s32 requested_alignment)
	{
		void* new_ptr = (void*)(alignUp((u32)ptr + (size - requested_size), requested_alignment));

		header* _header = get_header(new_ptr);
		_header->real_ptr = ptr;
		_header->real_size = requested_size;
		return new_ptr;
	}

	virtual void*		Allocate(s32 size)
	{
		s32 new_size = recalc_size(size, mDefaultAlignment);
		void* mem = MEMAllocFromAllocator(&mMemAllocator, new_size);
		++mAllocationCount;
		return set_header(mem, new_size, size, mDefaultAlignment);
	}

	virtual void*		reallocate(void* ptr, s32 size, u32 alignment)
	{
		header* _header = get_header(ptr);
		s32 new_size = recalc_size(size, alignment);
		void* mem = Allocate(size);
		::memcpy(mem, ptr, _header->real_size);
		Deallocate(ptr);
		return mem;
	}

	virtual void		Deallocate(void* ptr)
	{
		header* _header = get_header(ptr);
		MEMFreeToAllocator(&mMemAllocator, _header->real_ptr);
		--mAllocationCount;
	}

	virtual void		Release()
	{
		if (mAllocationCount != 0) {
			UnitTest::Stdout::Trace("ERROR: System Allocator is being released but still has allocations that are not freed\n");
			mAllocationCount = 0;
		}
		mArenaLo = NULL;
		mArenaHi = NULL;
	}
};

UnitTestAllocator*		gCreateSystemAllocator()
{
	static UnitTestAllocator	sWiiMem1SystemAllocator;
	static UnitTestAllocator	sWiiMem2SystemAllocator;

	if (sWiiMem1SystemAllocator.isInitialized() == false)
	{
		if (sWiiMem1SystemAllocator.initMem1() == true)
			return &sWiiMem1SystemAllocator;
	}
	else if (sWiiMem2SystemAllocator.isInitialized() == false)
	{
		if (sWiiMem2SystemAllocator.initMem2() == true)
			return &sWiiMem2SystemAllocator;
	}

	// System failure
	return NULL;
}

#endif