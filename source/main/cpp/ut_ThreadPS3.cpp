#include "xunittest\private\ut_ThreadPS3.h"

#include <stdio.h>

#if defined(TARGET_PS3)

namespace UnitTest
{
	Thread * gCreateThread( Runnable * inRunnable,
		const char * inName /*= NULL*/ )
	{
		if (inRunnable == NULL) 
		{
			return NULL;
		}

		ThreadPS3 * threadIns = new ThreadPS3;
		threadIns->mRunnable = inRunnable;

		int ret;
		threadIns->m_tid = SYS_PPU_THREAD_ID_INVALID;

		ret = sys_ppu_thread_create(
			&threadIns->m_tid,
			threadIns->_dispatch,
			(uint64_t)(uintptr_t)threadIns,
			1002, 0x10000,
			SYS_PPU_THREAD_CREATE_JOINABLE,
			NULL
		);

		if (ret < 0)
		{
			// @TODO: CREATE ERROR
			return NULL;
		}

		ThreadManager::instance()->addThread(threadIns, threadIns->m_tid);
		threadIns->m_thread_running = true;

		return threadIns;
	}

	Mutex * gCreateMutex()
	{
		return new MutexPS3();
	}

	Event * gCreateEvent()
	{
		return new EventPS3();
	}

	void gWaitForEvent(Event * inEvent, int inTimeOut /* = 0 */ )
	{
		usecond_t timeout = inTimeOut * 1000;
		EventPS3 * evt = static_cast<EventPS3 *>(inEvent);
		int ret = sys_semaphore_wait(evt->m_sem, timeout);
// 		switch(ret)
// 		{
// 		case CELL_OK:
// 			printf("The condition variable is successfully created\n");
// 			break;
// 		case ETIMEDOUT:
// 			printf("Expiration of the specified period\n");
// 			break;
// 		case ESRCH:
// 			printf("Invalid ID (The ID has not been issued by the kernel)\n");
// 			break;
// 		case EPERM:
// 			printf("The caller thread is not the owner of the mutex\n");
// 			break;
// 		default:
// 			printf("Fuck You\n");
// 			break;
// 		}
	}

	void gSleep(int inMiniSecond)
	{
		sys_timer_usleep(inMiniSecond * 1e3);
	}

	bool ThreadPS3::isTerminated()
	{
		return m_thread_running;
	}

	bool ThreadPS3::waitForExit()
	{
		int ret;
		uint64_t tstat;

		ret = sys_ppu_thread_join(this->m_tid, &tstat);
		if (ret != CELL_OK) 
		{
			printf("ERROR: sys_ppu_thread_join() failed. (%d)\n", ret);
			return false;
		}

		this->m_thread_running = false;
		return true;
	}

	void ThreadPS3::release()
	{
		if (!this->m_thread_running) 
		{
			ThreadManager::instance()->removeThread(this);
			this->m_thread_running = 0;
			delete this;
		}
	}

	void ThreadPS3::run()
	{
		if (mRunnable->init())
		{
			mRunnable->run();
			mRunnable->exit();
		}

		delete mRunnable;
		mRunnable = NULL;
	}

	void ThreadPS3::_dispatch(uint64_t arg)
	{
		ThreadPS3 * threadIns = (ThreadPS3*)(uintptr_t)arg;
		threadIns->run();
		sys_ppu_thread_exit(0);

		return;
	}

	void EventPS3::release()
	{
		sys_semaphore_destroy(this->m_sem);
		delete this;
	}

	bool EventPS3::signal()
	{
		sys_semaphore_post(this->m_sem, MAX_THREAD_NUMBER);
		return true;
	}

	void EventPS3::reset()
	{
		// don't know how to do it...
	}
}

#endif