//#if defined(TARGET_PC)

#ifndef __XUNITTEST_THREAD_H__
#define __XUNITTEST_THREAD_H__
// xunittest threading interface file

#define MAX_THREAD_NUMBER 10
#include "ut_Config.h"


#define CLASS_NEW_DELETE_OVERLOAD																										\
	void*	operator new(size_t num_bytes, void* mem)			{ return mem; }													\
	void	operator delete(void* mem, void* )					{ }																\
	void*	operator new(size_t num_bytes)						{ return __private::GetAllocator()->Allocate(num_bytes); }		\
	void	operator delete(void* mem)							{ __private::GetAllocator()->Deallocate(mem); }	

namespace UnitTest
{
	class Runnable
	{
	public:
		virtual bool init() { return true; }
		virtual void run() = 0;
		virtual void exit() {}

		virtual ~Runnable() { }
	};

 
	class Thread
	{
	public:
//   		virtual bool suspend() = 0;
//   		virtual bool resume() = 0;

  		virtual bool isTerminated() = 0;
 		virtual bool terminate() = 0; 
	 	virtual void release() = 0;
		virtual bool waitForExit() = 0;

		virtual ~Thread() { }
	};

	class Mutex
	{
	public:
		virtual void lock() = 0;
		virtual void unlock() = 0;
		virtual void release() = 0;

		virtual ~Mutex() { }
	};

  	class Event
  	{
  	public:
  		virtual bool signal() = 0;
  		virtual void reset() = 0;
  		virtual bool release() = 0;

		virtual ~Event() { }
  	};


	class ScopeLock
	{
	public:
		ScopeLock(Mutex * inMutex):mMutex(inMutex) 
		{
			mMutex->lock();
		}

		~ScopeLock()
		{
			mMutex->unlock();
		}

	private:

		Mutex *			mMutex;

		ScopeLock() {}
	};

	Thread * gCreateThread(Runnable * inRunnable, const char * inName = 0);

  	Mutex * gCreateMutex();
  	Event * gCreateEvent();
 
 	void gSleep(int inMiniSecond);
	bool gWaitForEvent(Event * inEvent, int inTimeOut = 0);
	
}



#endif//__XUNITTEST_THREAD_H__

//#endif