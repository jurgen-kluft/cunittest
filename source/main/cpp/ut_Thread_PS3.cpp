#if defined(TARGET_PS3)
#include "xunittest/private/ut_Thread_PS3.h"
#include <stdio.h>

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
			inName
		);

		if (ret < 0)
		{
			// @TODO: CREATE ERROR
			return NULL;
		}

		//ThreadManager::instance()->addThread(threadIns, threadIns->m_tid);

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

	bool gWaitForEvent(Event * inEvent, int inTimeOut /* = 0 */ )
	{
		usecond_t timeout = inTimeOut * 1000;
		EventPS3 * evt = static_cast<EventPS3 *>(inEvent);
		int ret = sys_semaphore_wait(evt->m_sem, timeout);
		return ret == CELL_OK;
	}

	void gSleep(int inMiniSecond)
	{
		sys_timer_usleep(inMiniSecond * 1000);
	}


	//---------------------------------------------------------
	// @@Thread
	//---------------------------------------------------------
	bool ThreadPS3::isTerminated()
	{
		return !m_thread_running;
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
			// ThreadManager::instance()->removeThread(this);

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



	//---------------------------------------------------------
	// @@Mutex
	//---------------------------------------------------------
	MutexPS3::MutexPS3()
	{
		sys_mutex_attribute_t mutex_attr;
		sys_mutex_attribute_initialize(mutex_attr);
		if(CELL_OK != sys_mutex_create(&m_mutex, &mutex_attr))
		{
			printf( "MutexPS3> sys_mutex_create failed.\n" );
		}
	}

	void MutexPS3::lock()
	{
		sys_mutex_lock(m_mutex, 0);
	}

	void MutexPS3::unlock()
	{
		sys_mutex_unlock(m_mutex);
	}

	void MutexPS3::release()
	{
		sys_mutex_destroy(m_mutex);
		delete this;
	}


	//-------------------------------------------------------------
	// @@EventWin32
	//-------------------------------------------------------------
	EventPS3::EventPS3()
	{
		sys_semaphore_attribute_t attr;
		sys_semaphore_attribute_initialize(attr);
		sys_semaphore_create(&m_sem, &attr, 0, MAX_THREAD_NUMBER);
	}

	bool EventPS3::release()
	{
		int ret = sys_semaphore_destroy(this->m_sem);
		delete this;
		return ret == CELL_OK;
	}

	bool EventPS3::signal()
	{
		int ret = sys_semaphore_post(this->m_sem, MAX_THREAD_NUMBER);
		return ret == CELL_OK;
	}

	void EventPS3::reset()
	{

	}
}

#endif