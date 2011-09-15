#include "xunittest\private\ut_ThreadBase.h"


namespace UnitTest
{
	bool ThreadManager::addThread( ThreadBase * inThread, ThreadHandle_t inHandle )
	{
		ThreadInfo tInfo;
		tInfo.ThreadInstance = inThread;
		tInfo.ThreadHandle = inHandle;

		ThreadList.push_back(tInfo);
		return true;
	}

	bool ThreadManager::removeThread( ThreadBase * inThread )
	{
		std::list<ThreadInfo>::iterator it;
		for (it = ThreadList.begin(); it != ThreadList.end(); it++) 
		{
			if ((*it).ThreadInstance == inThread) 
			{
				ThreadList.erase(it++);
				return true;
			}
		}
		return false;
	}

	ThreadBase * ThreadManager::getThreadByHandle(ThreadHandle_t inHandle)
	{
		std::list<ThreadInfo>::iterator it;
		for (it = ThreadList.begin(); it != ThreadList.end(); it++) 
		{
			if ((*it).ThreadHandle == inHandle) 
			{
				return (*it).ThreadInstance;
			}
		}
		return NULL;
	}

	bool ThreadManager::hasThread(ThreadBase * inThread)
	{
		std::list<ThreadInfo>::iterator it;
		for (it = ThreadList.begin(); it != ThreadList.end(); it++) 
		{
			if ((*it).ThreadInstance == inThread) 
			{
				return true;
			}
		}
		return false;
	}
}