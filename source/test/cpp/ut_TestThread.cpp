#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_TimeHelpers.h"
#include "xunittest\private\ut_Thread.h"
#include "xunittest\private\ut_Thread_Base.h"
#include "xunittest\private\ut_Stdout.h"
#include <stdio.h>
//#include <memory.h>

/*
CACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
--A--
--B--
--C--
A over
B over
C over
A timer: 4321
B timer: 4321
C timer: 4321
--D--
--E--
--F--
D over
E over
F over
*/

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestThreadSuite)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

 		struct MyRunnable : public Runnable
 		{
 			void run()
 			{
 				for (int i = 0; i < 100; ++i)
 				{
 					//Stdout::Trace("A");
 					gSleep(10);
 				}
 			}
 		};
 
 		// test thread creation
 		UNITTEST_TEST(TestTest)
 		{
 			Thread * threadIns = gCreateThread(new MyRunnable);
 
 			for (int i = 0; i < 210; ++i)
 			{
 				Stdout::Trace("C");
 				gSleep(10);
 			}
 			Stdout::Trace("\n");
 
 			threadIns->waitForExit();
 
 			threadIns->release();
 		}
 
   		Mutex * gMutex = NULL;
   
   		struct RunabWriteA : public Runnable
   		{
   			void run()
   			{
   				ScopeLock lock(gMutex);
  // 				gMutex->lock();
   				for (int i = 0; i < 128; i++)
   				{
   					//gBuff[i % 32] = 'A';
   					Stdout::Trace("A");
   					gSleep(10);
   				}
   				Stdout::Trace("\n");
  // 				gMutex->unlock();
   			}
   		};
   
   		struct RunabWriteB : public Runnable
   		{
   			void run()
   			{
   				ScopeLock lock(gMutex);
  // 				gMutex->lock();
   				for (int i = 0; i < 128; i++)
   				{
   					//gBuff[i % 32] = 'A';
   					Stdout::Trace("B");
   					gSleep(10);
   				}
   				Stdout::Trace("\n");
  // 				gMutex->unlock();
   			}
   		};
   
   		// test thread A and B can be asyned by using mutex
   		UNITTEST_TEST(TestAsynWrite)
   		{
   			gMutex = gCreateMutex();
   
   			Thread * threadInsA = gCreateThread(new RunabWriteA);
   			Thread * threadInsB = gCreateThread(new RunabWriteB);
   
   			threadInsA->waitForExit();
   			threadInsB->waitForExit();
   
   			for (int i = 0; i < 32; i++)
   			{
   				Stdout::Trace("M");
  				gSleep(10);
   			}
   
   			Stdout::Trace("\n");
   
   			gMutex->release();
   			threadInsA->release();
   			threadInsB->release();
   		}
  
  		Timer gTimer;
  
  		int gTimeThreadA = 0;
  		int gTimeThreadB = 0;
  		int gTimeThreadC = 0;
  
  		Event * gEvent = NULL;
  
  		struct RunabThreadA : public Runnable
  		{
  			void run()
  			{
  				gWaitForEvent(gEvent);
  				gTimeThreadA = gTimer.getTimeInMs();
  				Stdout::Trace("A over\n");
  			}
  		};
  
  		struct RunabThreadB : public Runnable
  		{
  			void run()
  			{
  				gWaitForEvent(gEvent);
  				gTimeThreadB = gTimer.getTimeInMs();
  				Stdout::Trace("B over\n");
  			}
  		};
  
  		struct RunabThreadC : public Runnable
  		{
  			void run()
  			{
  				gWaitForEvent(gEvent);
  				gTimeThreadC = gTimer.getTimeInMs();
  				Stdout::Trace("C over\n");
  			}
  		};
  
  		// test thread A, B and C will launch at the same time
  		UNITTEST_TEST(TestAwakenThread)
  		{
  			gTimer.start();
  
  			gEvent = gCreateEvent();
  
  			Thread * threadA = gCreateThread(new RunabThreadA);
  			Stdout::Trace("--A--\n");
  			Thread * threadB = gCreateThread(new RunabThreadB);
  			Stdout::Trace("--B--\n");
  			Thread * threadC = gCreateThread(new RunabThreadC);
  			Stdout::Trace("--C--\n");
  
  			//hangout for some mini-seconds
  			gSleep(4321);
  
  			gEvent->signal();
  
  			threadA->waitForExit();
  			threadB->waitForExit();
  			threadC->waitForExit();
  
 			char output[100];
  			Stdout::StringFormat(output, "A timer: %d\n", gTimeThreadA);
 			Stdout::Trace(output);
  			Stdout::StringFormat(output, "B timer: %d\n", gTimeThreadB);
 			Stdout::Trace(output);
  			Stdout::StringFormat(output, "C timer: %d\n", gTimeThreadC);
 			Stdout::Trace(output);
  
  			gEvent->release();
  			threadA->release();
  			threadB->release();
  			threadC->release();
  		}
 
// 		struct RunabThreadD : public Runnable
// 		{
// 			void run()
// 			{
// 				gSleep(1000);
// 				Stdout::Trace("D over\n");
// 			}
// 		};
// 
// 		struct RunabThreadE : public Runnable
// 		{
// 			void run()
// 			{
// 				gSleep(2000);
// 				Stdout::Trace("E over\n");
// 			}
// 		};
// 
// 		struct RunabThreadF : public Runnable
// 		{
// 			void run()
// 			{
// 				gSleep(4000);
// 				Stdout::Trace("F over\n");
// 			}
// 		};
// 
// 		// test thread A, B and C will launch at the same time
// 		UNITTEST_TEST(TestThreadManager)
// 		{
// 			gTimer.start();
// 
// 			gEvent = gCreateEvent();
// 
// 			Thread * threadD = gCreateThread(new RunabThreadD);
// 			Stdout::Trace("--D--\n");
// 			Thread * threadE = gCreateThread(new RunabThreadE);
// 			Stdout::Trace("--E--\n");
// 			Thread * threadF = gCreateThread(new RunabThreadF);
// 			Stdout::Trace("--F--\n");
// 
// 			CHECK_TRUE(ThreadManager::instance()->hasThread((ThreadBase*)threadD));
// 			CHECK_TRUE(ThreadManager::instance()->hasThread((ThreadBase*)threadE));
// 			CHECK_TRUE(ThreadManager::instance()->hasThread((ThreadBase*)threadF));
// 
// 			threadD->waitForExit();
// 			threadE->waitForExit();
// 			threadF->waitForExit();
// 
// 			threadD->release();
// 			threadE->release();
// 			threadF->release();
// 		}
	}

}

UNITTEST_SUITE_END