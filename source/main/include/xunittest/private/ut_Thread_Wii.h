#ifndef __XUNITTEST_THREAD_WII_H__
#define __XUNITTEST_THREAD_WII_H__

#if defined(TARGET_WII)

#include <revolution.h>
#include <revolution/os.h>
#include "ut_Thread_Base.h"
#include "ut_Config.h"

namespace UnitTest
{
// 	class MutexWii : public Mutex
// 	{
// 	public:
// 		MutexWii();
// 		virtual ~MutexWii();
// 		virtual void lock();
// 		virtual void unlock();
// 		virtual void release();
// 
// 	private:
// 	};
// 
// 	class EventWii : public Event
// 	{
// 	public:
// 		virtual bool signal();
// 		virtual void reset();
// 		virtual bool release();
// 
// 		virtual ~EventWii() { }
// 
// 		friend bool gWaitForEvent(Event * inEvent, int inTimeOut);
// 		friend Event * gCreateEvent();
// 	private:
// 		EventWii() {}
// 	};

	class ThreadWii : public ThreadBase
	{
	public:
// 		virtual bool suspend();
 		virtual bool resume();

 		virtual bool waitForExit();
		virtual void release();
// 		virtual bool isTerminated();
//		virtual bool terminate() { return false; }
//		virtual bool waitForEvent(Event * inEvent, int inTimeOut = 0);

		void init()
		{
			mThread = (OSThread*)__private::GetAllocator()->Allocate(sizeof(OSThread));
		}

		virtual ~ThreadWii() { }

		void run();

		friend Thread * gCreateThread(Runnable * inRunnable, const char * inName /* = 0 */);

		static	void*	_dispatch(void * inParam);

	private:
		OSThread*		mThread;
		int				mThreadId;

		bool			m_thread_running;
	};
}


#endif

#endif