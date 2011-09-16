#ifndef __XUNITTEST_THREAD_WIN32_H__
#define __XUNITTEST_THREAD_WIN32_H__

#if defined(TARGET_PC) || defined(TARGET_360)

#if defined(TARGET_PC)
#include <Windows.h>
#elif defined(TARGET_360)
#include <xtl.h>
#endif

#include "ut_ThreadBase.h"

namespace UnitTest
{
	class MutexWin32 : public Mutex
	{
	public:
		MutexWin32()
		{ 
			InitializeCriticalSection(&mCs);
			SetCriticalSectionSpinCount(&mCs, 5000);
		}

		virtual ~MutexWin32()
		{
			DeleteCriticalSection(&mCs);
		}

		virtual void lock()
		{
			EnterCriticalSection(&mCs);
		}

		virtual void unlock()
		{
			LeaveCriticalSection(&mCs);
		}

		virtual void release()
		{
			delete this;
		}
	private:
		CRITICAL_SECTION		mCs;
	};

	class EventWin32 : public Event
	{
	public:
		EventWin32(HANDLE inHandle):mHandle(inHandle) {}

		virtual bool signal();
		virtual void reset();
		virtual void release();

		HANDLE getHandle()  { return mHandle; }

		virtual ~EventWin32() { }
	private:
		HANDLE		mHandle;

		EventWin32():mHandle(NULL) {}
	};

	class ThreadWin32 : public ThreadBase
	{
	public:
// 		virtual bool suspend();
// 		virtual bool resume();
		virtual bool waitForExit();
		virtual bool isTerminated();
		virtual bool terminate() { return false; }
		virtual bool waitForEvent(Event * inEvent, int inTimeOut = 0);
		virtual void release();
		virtual ~ThreadWin32() { }

		void run();


		static	INT	__stdcall	_dispatch(void * inParam);

		HANDLE			mThreadHandle;
		DWORD			mThreadId;
	};

}

#endif



#endif //__XUNITTEST_THREAD_WIN32_H__