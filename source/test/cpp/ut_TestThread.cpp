#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_TimeHelpers.h"
#include "xunittest\private\ut_Thread.h"

#include <stdio.h>
//#include <memory.h>

/*
The result should be like:
CACACACAACACACCACACAACCACACACACACAACACCACAACACCACAACACCACACAACCACACACACAACACCACA
CACACACACAACACACCAACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACAC
ACACACACACACACACACACACACACACACACACACACAC
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
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
 					printf("A");
 					gSleep(10);
 				}
 			}
 		};
 
 		// test thread creation
 		UNITTEST_TEST(TestTest)
 		{
 			Thread * threadIns = gCreateThread(new MyRunnable);
 
 			for (int i = 0; i < 100; ++i)
 			{
 				printf("C");
 				gSleep(10);
 			}
 			printf("\n");
 
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
  					printf("A");
  					gSleep(10);
  				}
  				printf("\n");
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
  					printf("B");
  					gSleep(10);
  				}
  				printf("\n");
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
  				printf("M");
 				gSleep(10);
  			}
  
  			printf("\n");
  
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
 				printf("A over\n");
 			}
 		};
 
 		struct RunabThreadB : public Runnable
 		{
 			void run()
 			{
 				gWaitForEvent(gEvent);
 				gTimeThreadB = gTimer.getTimeInMs();
 				printf("B over\n");
 			}
 		};
 
 		struct RunabThreadC : public Runnable
 		{
 			void run()
 			{
 				gWaitForEvent(gEvent);
 				gTimeThreadC = gTimer.getTimeInMs();
 				printf("C over\n");
 			}
 		};
 
 		// test thread A, B and C will launch at the same time
 		UNITTEST_TEST(TestAwakenThread)
 		{
 			gTimer.start();
 
 			gEvent = gCreateEvent();
 
 			Thread * threadA = gCreateThread(new RunabThreadA);
 			printf("--A--\n");
 			Thread * threadB = gCreateThread(new RunabThreadB);
 			printf("--B--\n");
 			Thread * threadC = gCreateThread(new RunabThreadC);
 			printf("--C--\n");
 
 			//hangout for some mini-seconds
 			gSleep(4321);
 
 			gEvent->signal();
 
 			threadA->waitForExit();
 			threadB->waitForExit();
 			threadC->waitForExit();
 
 			printf("A timer: %d\n", gTimeThreadA);
 			printf("B timer: %d\n", gTimeThreadB);
 			printf("C timer: %d\n", gTimeThreadC);
 
 			gEvent->release();
 			threadA->release();
 			threadB->release();
 			threadC->release();
 		}
	}

}

UNITTEST_SUITE_END