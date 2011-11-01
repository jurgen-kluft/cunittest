#include "xunittest\private\ut_Thread_Wii.h"

#if defined(TARGET_WII)

#include "xunittest/private/ut_Config.h"
#define uint unsigned int

namespace UnitTest
{
	u8			ThreadStack[4096];
	Thread* gCreateThread(Runnable* inRunnable,
		const char* inName /* = NULL */)
	{
		if (inRunnable = NULL)
		{
			return NULL;
		}

		OSInit();
		VIInit();

		ThreadWii* threadIns = (ThreadWii*)GetAllocator()->Allocate(sizeof(ThreadWii));
		threadIns->init();
		threadIns->mRunnable = inRunnable;

		bool status = OSCreateThread(
			threadIns->mThread,
			ThreadWii::_dispatch,
			0,
			ThreadStack + sizeof(ThreadStack),
			sizeof(ThreadStack),
			31,
			0);

		if (!status)
		{
			// @TODO: ERROR
			return NULL;
		}

		threadIns->mThreadId = (uint)threadIns->mThread;
		int statues = OSResumeThread(threadIns->mThread);
		threadIns->m_thread_running = true;

		return threadIns;
	}

	void gSleep(int inMiniSecond)
	{
		OSSleepTicks(inMiniSecond);
	}

	bool ThreadWii::resume()
	{
		int statues = OSResumeThread(this->mThread);
		return !statues;
	}

	void ThreadWii::run()
	{
		if (mRunnable->init())
		{
			mRunnable->run();
			mRunnable->exit();
		}

		GetAllocator()->Deallocate(mRunnable);
		mRunnable = NULL;
	}

	void* ThreadWii::_dispatch(void* arg)
	{
		ThreadWii * threadIns = (ThreadWii*)arg;
		threadIns->run();

		return 0;
	}

	void ThreadWii::release()
	{
		if (!this->m_thread_running) 
		{
			this->m_thread_running = false;
			GetAllocator()->Deallocate(mThread);
			GetAllocator()->Deallocate(this);
		}
	}

	bool ThreadWii::waitForExit()
	{
		BOOL result = OSJoinThread(mThread, NULL);

		return result == true;
	}
}

#endif