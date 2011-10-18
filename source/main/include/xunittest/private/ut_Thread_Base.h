#ifndef __XUNITTEST_THREAD_BASE_H__
#define __XUNITTEST_THREAD_BASE_H__

#if defined(TARGET_PS3)
#include <sys/types.h>
#endif

#include <stdio.h>
//#include <list>
#include "ut_thread.h"

namespace UnitTest
{
// #if defined(TARGET_PS3)
// 	typedef			uint64_t		ThreadHandle_t;
// #elif defined(TARGET_PC) || defined(TARGET_360)
// 	typedef			unsigned long	ThreadHandle_t;
// #else
// 	typedef			OSThread		ThreadHandle_t;
// #endif


	class ThreadBase : public Thread
	{
	public:
		Runnable *				mRunnable;
		virtual ~ThreadBase() { }
	};

// 	class ThreadManager
// 	{
// 	public:
// 		struct ThreadInfo
// 		{
// 			ThreadBase *		ThreadInstance;
// 			ThreadHandle_t		ThreadHandle;
// 			bool				isUsed;
// 			ThreadInfo()
// 			{
// 				ThreadInstance = NULL;
// 				ThreadHandle = -1;
// 				isUsed = false;
// 			}
// 		};
// 
// 
// 		static ThreadManager * instance() 
// 		{
// 			static ThreadManager ins;
// 			return &ins;
// 		}
// 
// 
// 		bool addThread(ThreadBase * inThread, ThreadHandle_t inHandle);
// 
// 		bool removeThread(ThreadBase * inThread);
// 
// 		ThreadBase * getThreadByHandle(ThreadHandle_t inHandle);
// 
// 		bool hasThread(ThreadBase * inThread);
// 
// 	protected:
// 		ThreadInfo threadList[MAX_THREAD_NUMBER];
// 
// 	private:
// 		// to make sure there is only one ThreadManager.
// 		const ThreadManager operator=(const ThreadManager &b) { }
// 		ThreadManager() { }
// 		ThreadManager(const ThreadManager &b) { }
// 	};

}


#endif //__XUNITTEST_THREAD_BASE_H__