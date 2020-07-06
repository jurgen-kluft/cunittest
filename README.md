# xunittest

Cross platform unittest library

- xunittest
  - CHECK_EQUAL(expected, actual)
  - CHECK_NOT_EQUAL(expected, actual)
  - CHECK_TRUE(actual)
  - CHECK_FALSE(actual)
  - CHECK_NULL
  - CHECK_NOT_NULL

This library also detects memory leaks on a UNITTEST_TEST() scope as well as a FIXTURE_SETUP/FIXTURE_TEARDOWN scope.

## Test Registration

You do need to register your suite to be part of the list of unittests to run.

```c++
UNITTEST_SUITE_LIST(xYourUnitTest);

UNITTEST_SUITE_DECLARE(xYourUnitTest, doubly_linked_list);

```

## Main Entry

This is an example of a test application main entry where you can see 2 main points that need to be initialized by the user:

- xasserthandler::sRegisterHandler (to make sure that asserts are catched by the unittest framework)
- UnitTest::SetAllocator           (the unittest framework needs an allocator to allocate memory)

After that you need to call ``UNITTEST_SUITE_RUN(reporter, xYourUnitTest);``

```c++
bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
    xbase::x_Init();

#ifdef TARGET_DEBUG
    xcore::xasserthandler::sRegisterHandler(&gAssertHandler);
#endif

    xcore::xalloc*           systemAllocator = xcore::xalloc::get_system();
    xcore::UnitTestAllocator unittestAllocator(systemAllocator);
    UnitTest::SetAllocator(&unittestAllocator);

    xcore::console->write("Configuration: ");
    xcore::console->writeLine(TARGET_FULL_DESCR_STR);

    xcore::TestAllocator testAllocator(systemAllocator);
    gTestAllocator = &testAllocator;

    int r = UNITTEST_SUITE_RUN(reporter, xYourUnitTest);
    if (UnitTest::GetNumAllocations() != 0)
    {
        reporter.reportFailure(__FILE__, __LINE__, "xunittest", "memory leaks detected!");
        r = -1;
    }

    gTestAllocator->release();

    UnitTest::SetAllocator(NULL);

    xbase::x_Exit();
    return r == 0;
}
```
