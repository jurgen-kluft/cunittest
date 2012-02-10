#include "xunittest\private\ut_Thread_Win32.h"

#if defined(TARGET_PC) || defined(TARGET_360)

namespace UnitTest
{
	//---------------------------------------------------------
	// @@Global
	//---------------------------------------------------------
	Thread * gCreateThread( Runnable * inRunnable, const char * inName /*= NULL*/ )
	{
		if (inRunnable == NULL)
		{
			return NULL;
		}

		ThreadWin32 * threadIns = new ThreadWin32();
		threadIns->mRunnable = inRunnable;

		HANDLE hd;
		DWORD id;
		hd = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWin32::_dispatch, threadIns, CREATE_SUSPENDED, &id);
		if (hd == NULL)
		{
			return NULL;
		}

		threadIns->mThreadHandle = hd;
		threadIns->mThreadId = id;

		ResumeThread(threadIns->mThreadHandle);

		return threadIns;
	}

	Mutex * gCreateMutex()
	{
		MutexWin32 * mutex = new MutexWin32();
		return mutex;
	}

	Event * gCreateEvent()
	{
		HANDLE hd = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hd == NULL)
		{
			//@TODO: HANDLE ERROR
			return NULL;
		}

		EventWin32 * event = new EventWin32(hd);
		return event;
	}

	void gSleep(int inMiniSecond)
	{
		Sleep(inMiniSecond);
	}

	bool gWaitForEvent(Event * inEvent, int inTimeOut/* = 0*/)
	{
		DWORD timeout = !inTimeOut ? INFINITE : inTimeOut;
		EventWin32 * evt = static_cast<EventWin32 *>(inEvent);
		DWORD dw = WaitForSingleObject(evt->getHandle(), timeout);
		switch (dw)
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			break;
		}
		return false;
	}

	//---------------------------------------------------------
	// @@Thread
	//---------------------------------------------------------
	INT	__stdcall ThreadWin32::_dispatch( void * inParam )
	{
		ThreadWin32 * threadIns = static_cast<ThreadWin32 *>(inParam);
		threadIns->run();

		return 0;
	}

	void ThreadWin32::run()
	{
		if (mRunnable->init())
		{
			mRunnable->run();
			mRunnable->exit();
		}

		delete mRunnable;
		mRunnable = NULL;
	}

	void ThreadWin32::release()
	{
		if (isTerminated())
		{
			CloseHandle(mThreadHandle);
			delete this;
		}
	}

	bool ThreadWin32::suspend()
	{
		DWORD dw = SuspendThread(mThreadHandle);
		if (dw == -1)
		{
			//@TODO: HANDLE ERROR
			return false;
		}
		return true;
	}

	bool ThreadWin32::resume()
	{
		DWORD dw = ResumeThread(mThreadHandle);
		if (dw == -1)
		{
			//@TODO: HANDLE ERROR
			return false;
		}

		return true;
	}

	bool ThreadWin32::waitForExit()
	{
		// a thread should not wait for itself
		HANDLE hd = GetCurrentThread();
		if (hd == mThreadHandle)
		{
			return false;
		}
		
		DWORD timeout = INFINITE;
		DWORD dw = WaitForSingleObject(mThreadHandle, timeout);
		switch (dw)
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			break;
		}

		return false;
	}

	bool ThreadWin32::isTerminated()
	{
		DWORD dw = WaitForSingleObject(mThreadHandle, 0);
		switch (dw)
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			break;
		}

		return false;
	}

	bool ThreadWin32::waitForEvent( Event * inEvent, int inTimeOut /*= 0*/ )
	{
		if (! inEvent)
		{
			return false;
		}

		DWORD timeout = !inTimeOut ? INFINITE : inTimeOut;
		EventWin32 * evt = static_cast<EventWin32 *>(inEvent);
		DWORD dw = WaitForSingleObject(evt->getHandle(), timeout);
		switch (dw)
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			break;
		}

		return false;
	}


	//---------------------------------------------------------
	// @@Mutex
	//---------------------------------------------------------
	MutexWin32::MutexWin32()
	{ 
		InitializeCriticalSection(&mCs);
		SetCriticalSectionSpinCount(&mCs, 5000);
	}

	MutexWin32::~MutexWin32()
	{
		DeleteCriticalSection(&mCs);
	}

	void MutexWin32::lock()
	{
		EnterCriticalSection(&mCs);
	}

	void MutexWin32::unlock()
	{
		LeaveCriticalSection(&mCs);
	}

	void MutexWin32::release()
	{
		delete this;
	}


	//-------------------------------------------------------------
	// @@EventWin32
	//-------------------------------------------------------------
	bool EventWin32::release()
	{
		bool ret = CloseHandle(mHandle) ? true : false;
		delete this;
		return ret;
	}

	bool EventWin32::signal()
	{
		return SetEvent(mHandle) ? true : false;
	}

	void EventWin32::reset()
	{
		ResetEvent(mHandle);
	}

}

#endif