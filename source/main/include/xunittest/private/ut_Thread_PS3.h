#ifndef __XUNITTEST_THREAD_WIN32_H__
#define __XUNITTEST_THREAD_WIN32_H__

#if defined(TARGET_PS3)

#include <sys/ppu_thread.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/synchronization.h>
#include "ut_Thread_Base.h"


namespace UnitTest 
{
	class MutexPS3 : public Mutex
	{
	public:
		MutexPS3();
		virtual ~MutexPS3() { }
		virtual void lock();
		virtual void unlock();
		virtual void release();

	private:
		sys_mutex_t m_mutex;
	};

	class EventPS3 : public Event
	{
	public:
		virtual bool signal();
		virtual void reset();
		virtual bool release();

		virtual ~EventPS3() { }

		friend bool gWaitForEvent(Event * inEvent, int inTimeOut);
		friend Event * gCreateEvent();

	private:
		EventPS3();

		sys_semaphore_t m_sem;
	};

	class ThreadPS3 : public ThreadBase
	{
	public:
		virtual bool waitForExit();
		virtual void release();
		virtual bool isTerminated();
		virtual bool terminate() { return false; }

		virtual ~ThreadPS3() { }

		void run();

		friend Thread * gCreateThread(Runnable * inRunnable, const char * inName /* = 0 */);

		static void _dispatch(uint64_t arg);
	private:
		sys_ppu_thread_t m_tid;
		bool m_thread_running;
	};
}


#endif



#endif //__XUNITTEST_THREAD_WIN32_H__