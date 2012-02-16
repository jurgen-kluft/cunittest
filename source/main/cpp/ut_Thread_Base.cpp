#include "xunittest\private\ut_Thread_Base.h"


namespace UnitTest
{
// 	bool ThreadManager::addThread( ThreadBase * inThread, ThreadHandle_t inHandle )
// 	{
// 		for (int i = 0; i < MAX_THREAD_NUMBER; i++)
// 		{
// 			if (!threadList[i].isUsed)
// 			{
// 				threadList[i].ThreadInstance = inThread;
// 				threadList[i].ThreadHandle = inHandle;
// 				threadList[i].isUsed = true;
// 				return true;
// 			}
// 		}
// 		return false;
// 	}
// 
// 	bool ThreadManager::removeThread( ThreadBase * inThread )
// 	{
// 		for (int i = 0; i < MAX_THREAD_NUMBER; i++)
// 		{
// 			if (threadList[i].isUsed && inThread == threadList[i].ThreadInstance)
// 			{
// 				threadList[i].isUsed = false;
// 				return true;
// 			}
// 		}
// 		return false;
// 	}
// 
// 	ThreadBase * ThreadManager::getThreadByHandle(ThreadHandle_t inHandle)
// 	{
// 		for (int i = 0; i < MAX_THREAD_NUMBER; i++)
// 		{
// 			if (threadList[i].isUsed && threadList[i].ThreadHandle == inHandle)
// 			{
// 				return threadList[i].ThreadInstance;
// 			}
// 		}
// 		return NULL;
// 	}
// 
// 	bool ThreadManager::hasThread(ThreadBase * inThread)
// 	{
// 		for (int i = 0; i < MAX_THREAD_NUMBER; i++)
// 		{
// 			if (threadList[i].isUsed && threadList[i].ThreadInstance == inThread)
// 			{
// 				return true;
// 			}
// 		}
// 		return false;
// 	}
}