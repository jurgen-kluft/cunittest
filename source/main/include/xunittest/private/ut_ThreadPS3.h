#ifndef __XUNITTEST_THREAD_WIN32_H__
#define __XUNITTEST_THREAD_WIN32_H__

#if defined(TARGET_PS3)

#include <sys/ppu_thread.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/synchronization.h>
#include "ut_ThreadBase.h"


namespace UnitTest 
{
	class MutexPS3 : public Mutex
	{
	public:
		MutexPS3()
		{
			sys_mutex_attribute_t mutex_attr;
			sys_mutex_attribute_initialize(mutex_attr);
			if( CELL_OK != sys_mutex_create(&m_mutex, &mutex_attr))
			{
				printf( "MutexPS3> sys_mutex_create failed.\n" );
			}
		}

		virtual ~MutexPS3() { }

		virtual void lock()
		{
			sys_mutex_lock(m_mutex, 0);
		}

		virtual void unlock()
		{
			sys_mutex_unlock(m_mutex);
		}

		virtual void release()
		{
			sys_mutex_destroy(m_mutex);
			delete this;
		}

	private:
		sys_mutex_t m_mutex;
	};

	class EventPS3 : public Event
	{
	public:
		virtual bool signal();
		virtual void reset();
		virtual void release();

		virtual ~EventPS3() { }

		friend void gWaitForEvent(Event * inEvent, int inTimeOut);
		friend Event * gCreateEvent();

	private:
		EventPS3()
		{
			sys_semaphore_attribute_t attr;
			sys_semaphore_attribute_initialize(attr);
			sys_semaphore_create(&m_sem, &attr, 0, MAX_THREAD_NUMBER);
		}

		sys_semaphore_t m_sem;
	};

	class ThreadPS3 : public ThreadBase
	{
	public:
/*		virtual bool waitForExit(int inTimeOut = 0);*/

		// No need to suspend or resume the thread
		// there is no need here...
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