#ifndef __XUNITTEST_THREAD_WII_H__
#define __XUNITTEST_THREAD_WII_H__

#if defined(TARGET_WII)

#include <revolution.h>
#include <revolution/os.h>
#include "ut_Thread_Base.h"
#include "ut_Config.h"

namespace UnitTest
{
	class EventWII : public Event
	{
	public:
		EventWII();
		virtual ~EventWII() {}

		virtual bool signal();
		virtual void reset();
		virtual bool release();

		CLASS_NEW_DELETE_OVERLOAD

			friend bool gWaitForEvent(Event * inEvent, int inTimeOut);

	private:
		OSCond  m_condition;
		OSMutex m_mutex;
	};

	class MutexWII : public Mutex
	{
	public:
		MutexWII();
		virtual ~MutexWII() {}

		virtual void lock();
		virtual void unlock();
		virtual void release();

		CLASS_NEW_DELETE_OVERLOAD

	private:
		OSMutex m_mutext;
	};

	class ThreadWii : public ThreadBase
	{
	public:
		ThreadWii() : mThreadStack(NULL), mThreadId(-1), m_thread_running(false) {}
		virtual				~ThreadWii() { }

		virtual bool		resume();

		virtual bool		isTerminated()		{ return !m_thread_running; }
		virtual bool		terminate()			{ return false; }


		virtual bool		waitForExit();
		virtual void		release();

		void				init()	{ }
		void				run();

		friend Thread*		gCreateThread(Runnable * inRunnable, const char * inName /* = 0 */);
		static	void*		_dispatch(void * inParam);

		CLASS_NEW_DELETE_OVERLOAD;

	private:
		char*			mThreadStack;
		OSThread		mThread;
		unsigned int	mThreadId;

		bool			m_thread_running;
	};
}


#endif

#endif