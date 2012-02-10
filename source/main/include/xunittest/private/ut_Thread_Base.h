#ifndef __XUNITTEST_THREAD_BASE_H__
#define __XUNITTEST_THREAD_BASE_H__

#include <stdio.h>
#include "ut_thread.h"

namespace UnitTest
{
	class ThreadBase : public Thread
	{
	public:
		Runnable*		mRunnable;
		virtual			~ThreadBase() { }
	};
}


#endif //__XUNITTEST_THREAD_BASE_H__