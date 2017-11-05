#ifndef __XUNITTEST_THREAD_MACOS_H__
#define __XUNITTEST_THREAD_MACOS_H__

#if defined(TARGET_MACOS)

#include <pthread.h>
#include "ut_Thread_Base.h"
#include "ut_Config.h"

namespace UnitTest 
{
	class MutexOSX : public Mutex
	{
	public:
		MutexOSX();
		virtual ~MutexOSX() { }
		virtual void lock();
		virtual void unlock();
		virtual void release();

		CLASS_NEW_DELETE_OVERLOAD;

	private:
		pthread_mutex_t m;
	};

	class EventOSX : public Event
	{
	public:
		virtual bool signal();
		virtual void reset();
		virtual bool release();

		CLASS_NEW_DELETE_OVERLOAD;

		virtual ~EventOSX() { }

		friend bool gWaitForEvent(Event * inEvent, int inTimeOut);
		friend Event * gCreateEvent();

	private:
		EventOSX();

		pthread_mutex_t m;
		pthread_cond_t c;
		volatile int state;
	};

	class ThreadOSX : public ThreadBase
	{
	public:
		virtual bool waitForExit();
		virtual void release();
		virtual bool isTerminated();
		virtual bool terminate() { return false; }

		CLASS_NEW_DELETE_OVERLOAD;

		virtual ~ThreadOSX() { }

		void run();

		friend Thread * gCreateThread(Runnable * inRunnable, const char * inName /* = 0 */);

		static void _dispatch(void* arg);
	private:
		pthread_t m_tid;
		bool m_thread_running;
	};
}


#endif



#endif //__XUNITTEST_THREAD_MACOS_H__