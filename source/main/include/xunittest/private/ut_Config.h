#ifndef __XUNITTEST_CONFIG_H__
#define __XUNITTEST_CONFIG_H__

namespace UnitTest
{
	class Allocator
	{
	public:
		virtual				~Allocator() {}

		virtual void*		Allocate(int size) = 0;
		virtual void		Deallocate(void* ptr) = 0;
	};

	class CountingAllocator : public Allocator
	{
	public:
		Allocator*	mAllocator;

		CountingAllocator(Allocator* allocator) : mAllocator(allocator)
		{
		}

		virtual void*		Allocate(int size) { mNumAllocations++; return mAllocator->Allocate(size); }
		virtual void		Deallocate(void* ptr) {mNumAllocations--; mAllocator->Deallocate((ptr)); }

		int			mNumAllocations;
	};

	extern void			SetCountingAllocator(Allocator* allocator);
	extern CountingAllocator*	GetCountingAllocator();


	class Observer
	{
	public:
		virtual				~Observer() {}

		virtual void		BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) = 0;
		virtual void		EndFixture() = 0;
	};

	extern void			SetObserver(Observer* observer);
	extern Observer*	GetObserver();
}


#endif	///< __XUNITTEST_CONFIG_H__
