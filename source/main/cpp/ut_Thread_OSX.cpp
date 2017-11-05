#if defined(TARGET_MACOS)
#include "xunittest/private/ut_Thread_OSX.h"
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

		ThreadOSX * threadIns = new ThreadOSX;
		threadIns->mRunnable = inRunnable;

		int ret = 0;
		threadIns->m_tid = 0;

		ret = pthread_create(&threadIns->m_tid, NULL, threadIns->_dispatch, info);
		pthread_detach(threadIns->m_tid);
	
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
		return new MutexOSX();
	}

	Event * gCreateEvent()
	{
		return new EventOSX();
	}

	bool gWaitForEvent(Event * inEvent, int inTimeOut /* = 0 */ )
	{
		EventOSX * e = static_cast<EventOSX *>(inEvent);

		pthread_mutex_lock(&e->m);
		if (e->state == 1) {
			e->state = 0;
			pthread_mutex_unlock(&e->m);
			return true;
		}
		const struct timespec *restrict abstime = NULL;
		pthread_cond_timedwait(&e->c, &e->m, abstime);
		e->state = 0;
		pthread_mutex_unlock(&e->m);

		return true;
	}

	void gSleep(int inMilliSeconds)
	{
		sleep(inMiniSecond);
	}


	//---------------------------------------------------------
	// @@Thread
	//---------------------------------------------------------
	bool ThreadOSX::isTerminated()
	{
		return !m_thread_running;
	}

	bool ThreadOSX::waitForExit()
	{
		int ret;
		uint64_t tstat;

		ret = pthread_join(this->m_tid, &tstat);
		if (ret != CELL_OK) 
		{
			printf("ERROR: sys_ppu_thread_join() failed. (%d)\n", ret);
			return false;
		}

		this->m_thread_running = false;
		return true;
	}

	void ThreadOSX::release()
	{
		if (!this->m_thread_running) 
		{
			// ThreadManager::instance()->removeThread(this);

			this->m_thread_running = 0;
			delete this;
		}
	}

	void ThreadOSX::run()
	{
		if (mRunnable->init())
		{
			mRunnable->run();
			mRunnable->exit();
		}

		delete mRunnable;
		mRunnable = NULL;
	}

	void ThreadOSX::_dispatch(void* arg)
	{
		ThreadOSX * threadIns = (ThreadOSX*)arg;
		threadIns->run();
		pthread_exit();
		return;
	}



	//---------------------------------------------------------
	// @@Mutex
	//---------------------------------------------------------
	MutexOSX::MutexOSX()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, RECURSIVE_MUTEX);
		pthread_mutex_init(&m, &attr);
		pthread_mutexattr_destroy(&attr);
	}

	void MutexOSX::lock()
	{
		pthread_mutex_lock(m, 0);
	}

	void MutexOSX::unlock()
	{
		pthread_mutex_unlock(m);
	}

	void MutexOSX::release()
	{
		pthread_mutex_destroy(m);
		delete this;
	}


	//-------------------------------------------------------------
	// @@EventWin32
	//-------------------------------------------------------------
	EventOSX::EventOSX()
	{
		pthread_mutex_init(&m, NULL);
		pthread_cond_init(&c, NULL);
		state = 0;
	}

	bool EventOSX::release()
	{
		pthread_cond_destroy(&c);
		pthread_mutex_destroy(&m);

		delete this;
		return true;
	}

	bool EventOSX::signal()
	{
		pthread_mutex_lock(&m);
		state = 1;
		pthread_mutex_unlock(&m);
		pthread_cond_signal(&c);
		return true;			
	}

	void EventOSX::reset()
	{

	}
}

#endif