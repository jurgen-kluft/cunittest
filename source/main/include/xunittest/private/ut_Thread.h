#if defined(TARGET_PC)

#ifndef __XUNITTEST_THREAD_H__
#define __XUNITTEST_THREAD_H__
// xunittest threading interface file


namespace UnitTest
{
	class Runnable
	{
	public:
		virtual bool init() { return true; }
		virtual void run() = 0;
		virtual void exit() {}
	};

	class Event
	{
	public:
		virtual bool signal() = 0;
		virtual void reset() = 0;
		virtual void release() = 0;
	};

	class Thread
	{
	public:
		virtual bool suspend() = 0;
		virtual bool resume() = 0;
		virtual bool waitForExit(int inTimeOut = 0) = 0;
		virtual bool isTerminated() = 0;
		virtual bool terminate() = 0; 
		virtual void release() = 0;
	};

	class Mutex
	{
	public:
		virtual void lock() = 0;
		virtual void unlock() = 0;
		virtual void release() = 0;
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
		bool inCreateSuspend = false,
		const char * inName = 0);

	Mutex * gCreateMutex();
	Event * gCreateEvent();

	void gSleep(int inMiniSecond);
	void gWaitForEvent(Event * inEvent, int inTimeOut = 0);
	
}



#endif//__XUNITTEST_THREAD_H__

#endif