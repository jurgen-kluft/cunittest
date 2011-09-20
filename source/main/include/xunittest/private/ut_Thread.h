//#if defined(TARGET_PC)

#ifndef __XUNITTEST_THREAD_H__
#define __XUNITTEST_THREAD_H__
// xunittest threading interface file

#define MAX_THREAD_NUMBER 10

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

  	class Event
  	{
  	public:
  		virtual bool signal() = 0;
  		virtual void reset() = 0;
  		virtual bool release() = 0;

		virtual ~Event() { }
  	};
 
	class Thread
	{
	public:
//  		virtual bool suspend() = 0;
//  		virtual bool resume() = 0;
  		virtual bool waitForExit() = 0;
  		virtual bool isTerminated() = 0;
  		virtual bool terminate() = 0; 
  		virtual void release() = 0;

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

	Thread * gCreateThread(Runnable * inRunnable,
		const char * inName = 0);

  	Mutex * gCreateMutex();
  	Event * gCreateEvent();
 
 	void gSleep(int inMiniSecond);
 	bool gWaitForEvent(Event * inEvent, int inTimeOut = 0);
	
}



#endif//__XUNITTEST_THREAD_H__

//#endif