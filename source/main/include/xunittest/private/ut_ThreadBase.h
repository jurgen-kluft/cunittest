#ifndef __XUNITTEST_THREAD_BASE_H__
#define __XUNITTEST_THREAD_BASE_H__

#include "ut_thread.h"
#include "ut_Common.h"

namespace UnitTest
{
#if defined(TARGET_PS3)
	typedef			uint64_t		ThreadHandle_t;
#else
	typedef			unsigned long	ThreadHandle_t;
#endif


	class ThreadBase : public Thread
	{
	public:
		Runnable *				mRunnable;
	};

	

	class ThreadManager
	{
	public:
		struct ThreadInfo
		{
			ThreadBase *		ThreadInstance;
			ThreadHandle_t		ThreadHandle;
		};

		static ThreadManager * instance() 
		{
			static ThreadManager ins;
			return &ins;
		}

		bool addThread(ThreadBase * inThread, ThreadHandle_t inHandle);

		bool removeThread(ThreadBase * inThread);

		ThreadBase * getThreadByHandle(ThreadHandle_t inHandle);

		bool hasThread(ThreadBase * InThread);

	protected:

		Array<ThreadInfo>		ThreadList;
	};

}


#endif //__XUNITTEST_THREAD_BASE_H__