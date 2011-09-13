#include "xunittest\private\ut_ThreadWin32.h"



namespace UnitTest
{
	Thread * gCreateThread( Runnable * inRunnable, 
		bool inCreateSuspend /*= false*/, 
		const char * inName /*= NULL*/ )
	{
		if (inRunnable == NULL)
		{
			return NULL;
		}

		ThreadWin32 * threadIns = new ThreadWin32;
		threadIns->mRunnable = inRunnable;

		HANDLE hd;
		DWORD id;
		hd = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWin32::_dispatch, threadIns, CREATE_SUSPENDED, &id);
		if (hd == NULL)
		{
			//@TODO: HANDLE ERROR
			return NULL;
		}

		threadIns->mThreadHandle = hd;
		threadIns->mThreadId = id;

		// add new thread to manager
		ThreadManager::instance()->addThread(threadIns, (ThreadHandle_t)hd);
		
		if (inCreateSuspend == false)
		{
			threadIns->resume();
		}

		return threadIns;
	}

	Mutex * gCreateMutex()
	{
		return new MutexWin32();
	}

	Event * gCreateEvent()
	{
		HANDLE hd = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hd == NULL)
		{
			//@TODO: HANDLE ERROR
			return NULL;
		}

		return new EventWin32(hd);
	}

	void gSleep(int inMiniSecond)
	{
		Sleep(inMiniSecond);
	}

	void gWaitForEvent(Event * inEvent, int inTimeOut/* = 0*/)
	{
		DWORD timeout = !inTimeOut ? INFINITE : inTimeOut;
		EventWin32 * evt = static_cast<EventWin32 *>(inEvent);
		DWORD dw = WaitForSingleObject(evt->getHandle(), timeout);
		switch (dw)
		{
		case WAIT_OBJECT_0:
			return ;
		default:
			break;
		}
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
			ThreadManager::instance()->removeThread(this);
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

	bool ThreadWin32::waitForExit( int inTimeOut /*= 0*/ )
	{
		// a thread should not wait for itself
		HANDLE hd = GetCurrentThread();
		if (hd == mThreadHandle)
		{
			return false;
		}
		
		DWORD timeout = !inTimeOut ? INFINITE : inTimeOut;
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





	//-------------------------------------------------------------
	// @@EventWin32
	//-------------------------------------------------------------
	void EventWin32::release()
	{
		CloseHandle(mHandle);
		delete this;
	}

	bool EventWin32::signal()
	{
		return SetEvent(mHandle);
	}

	void EventWin32::reset()
	{
		ResetEvent(mHandle);
	}

}