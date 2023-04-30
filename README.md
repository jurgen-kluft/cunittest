# cunittest

Cross platform unittest library

- Windows
- Mac OS

## Memory Leaks

Each fixture and test in a fixture is guarded against allocation/deallocation and the user will be informed if a test or a fixture is leaking memory.

## Assert Handler

If your library has the ability to internally handle asserts you can let the UnitTest library be aware of this.
This allows you to have a test fail if asserts are triggered within the code you are testing.

## Extending to other platforms

To extend for an additional platform, e.g. Linux, add:

- `source/main/cpp/entry/ut_Entry_Linux.cpp`
- `source/main/cpp/ut_Stdout_Linux.cpp`
- `source/main/cpp/ut_Test_Linux.cpp`
- `source/main/cpp/ut_TimeHelpers_Linux.cpp`
- `source/main/cpp/ut_Utils_Linux.cpp`

## Unittest MACROs

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
UNITTEST_SUITE_LIST(cUnitTest);

UNITTEST_SUITE_DECLARE(cUnitTest, doubly_linked_list);

```

## Main Entry

This is an example of a test application main entry where you can see 2 main points that need to be initialized by the user:

- context_t::set_assert_handler    (to make sure that asserts are catched by the unittest framework)
- UnitTest::SetAllocator           (the unittest framework needs an allocator to allocate memory)

After that you need to call ``UNITTEST_SUITE_RUN(context, reporter, cYourUnitTest);``

```c++
bool gRunUnitTest(UnitTest::TestReporter& reporter, UnitTest::TestContext& context)
{
    cbase::init();

#ifdef TARGET_DEBUG
    ncore::UnitTestAssertHandler assertHandler;
    ncore::context_t::set_assert_handler(&assertHandler);
#endif
    ncore::console->write("Configuration: ");
    ncore::console->setColor(ncore::console_t::YELLOW);
    ncore::console->writeLine(TARGET_FULL_DESCR_STR);
    ncore::console->setColor(ncore::console_t::NORMAL);

    ncore::alloc_t*          systemAllocator = ncore::context_t::system_alloc();
    ncore::UnitTestAllocator unittestAllocator(systemAllocator);
    context.mAllocator = &unittestAllocator;

    ncore::TestAllocator testAllocator(&unittestAllocator);
    ncore::context_t::set_system_alloc(&testAllocator);

    int r = UNITTEST_SUITE_RUN(context, reporter, cUnitTest);
    if (unittestAllocator.mNumAllocations != 0)
    {
        reporter.reportFailure(__FILE__, __LINE__, "cunittest", "memory leaks detected!");
        r = -1;
    }

    ncore::context_t::set_system_alloc(systemAllocator);

    cbase::exit();
    return r == 0;
}
```
