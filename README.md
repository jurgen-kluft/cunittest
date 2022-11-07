# cunittest

Cross platform unittest library

- cunittest
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
UNITTEST_SUITE_LIST(cYourUnitTest);

UNITTEST_SUITE_DECLARE(cYourUnitTest, doubly_linked_list);

```

## Main Entry

This is an example of a test application main entry where you can see 2 main points that need to be initialized by the user:

- context_t::set_assert_handler    (to make sure that asserts are catched by the unittest framework)
- UnitTest::SetAllocator           (the unittest framework needs an allocator to allocate memory)

After that you need to call ``UNITTEST_SUITE_RUN(reporter, cYourUnitTest);``

```c++
bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
    cbase::init();

#ifdef TARGET_DEBUG
    ncore::context_t::set_assert_handler(&gAssertHandler);
#endif

    ncore::alloc_t* systemAllocator = ncore::context_t::system_alloc();
    ncore::UnitTestAllocator unittestAllocator(systemAllocator);
    UnitTest::SetAllocator(&unittestAllocator);

    ncore::console->write("Configuration: ");
    ncore::console->writeLine(TARGET_FULL_DESCR_STR);

    ncore::TestAllocator testAllocator(systemAllocator);
    gTestAllocator = &testAllocator;

    int r = UNITTEST_SUITE_RUN(reporter, cYourUnitTest);
    if (UnitTest::GetNumAllocations() != 0)
    {
        reporter.reportFailure(__FILE__, __LINE__, "cunittest", "memory leaks detected!");
        r = -1;
    }

    gTestAllocator->release();

    UnitTest::SetAllocator(nullptr);

    cbase::exit();
    return r == 0;
}
```
