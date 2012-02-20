#ifndef __XUNITTEST_THREAD_WII_H__
#define __XUNITTEST_THREAD_WII_H__

#if defined(TARGET_WII)

#include <revolution.h>
#include <revolution/os.h>
#include "ut_Thread_Base.h"
#include "ut_Config.h"

namespace UnitTest
{
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