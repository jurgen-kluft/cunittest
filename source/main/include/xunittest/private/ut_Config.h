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

	extern void			SetAllocator(Allocator* allocator);
	
	extern void			ResetNumAllocations();
	extern void			IncNumAllocations();
	extern void			DecNumAllocations();
	extern int			GetNumAllocations();

	class Observer
	{
	public:
		virtual				~Observer() {}

		virtual void		BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) = 0;
		virtual void		EndFixture() = 0;
	};

	extern void			SetObserver(Observer* observer);
	extern Observer*	GetObserver();

	namespace __private
	{
		extern Allocator*	GetAllocator();
	}
}


#endif	///< __XUNITTEST_CONFIG_H__
