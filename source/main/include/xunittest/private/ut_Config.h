#ifndef __XUNITTEST_CONFIG_H__
#define __XUNITTEST_CONFIG_H__

namespace UnitTest
{
#ifdef TARGET_64BIT
	#ifdef TARGET_MACOS
		typedef unsigned long long		size_t;
	#else
		typedef unsigned __int64		size_t;
	#endif
#else
	typedef unsigned int			size_t;
#endif
	class Allocator
	{
	public:
		virtual				~Allocator() {}

		virtual void*		Allocate(size_t size) = 0;
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


	class NullAllocator : public Allocator
	{
	public:
							NullAllocator() { }

		virtual void*		Allocate(size_t size) { return 0; }
		virtual void		Deallocate(void* ptr) {}
		void				Release()		{}
	};
	

	class NullObserver : public Observer
	{
	public:
		virtual void		BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) {}
		virtual void		EndFixture() {}

	};


	namespace __private
	{
		Allocator*		GetAllocator();
	}



}


#endif	///< __XUNITTEST_CONFIG_H__
