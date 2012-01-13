#if defined(TARGET_WII)
#include "xunittest\private\ut_Thread_Wii.h"
#include "xunittest\private\ut_Config.h"

namespace UnitTest
{
	Thread*		gCreateThread(Runnable* inRunnable, const char* inName /* = NULL */)
	{
		if (inRunnable = NULL)
		{
			return NULL;
		}

		OSInit();
		VIInit();

		ThreadWii* threadIns = new ThreadWii();
		threadIns->init();
		threadIns->mRunnable = inRunnable;

		threadIns->mThreadStack = (char*)__private::GetAllocator()->Allocate(4096);
		bool status = OSCreateThread(
			&threadIns->mThread,
			ThreadWii::_dispatch,
			0,
			threadIns->mThreadStack + sizeof(4096),
			4096,
			31,
			0);

		if (!status)
		{
			// @TODO: ERROR
			return NULL;
		}

		threadIns->mThreadId = (unsigned int)&threadIns->mThread;
		int statues = OSResumeThread(&threadIns->mThread);
		threadIns->m_thread_running = true;

		return threadIns;
	}

	void gSleep(int inMiniSecond)
	{
		OSSleepTicks(inMiniSecond);
	}

	bool ThreadWii::resume()
	{
		int statues = OSResumeThread(&mThread);
		return !statues;
	}

	void ThreadWii::run()
	{
		if (mRunnable->init())
		{
			mRunnable->run();
			mRunnable->exit();
		}

		delete mRunnable;
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
			__private::GetAllocator()->Deallocate(mThreadStack);
			delete this;
		}
	}

	bool ThreadWii::waitForExit()
	{
		BOOL result = OSJoinThread(&mThread, NULL);
		return result == true;
	}
}

#endif