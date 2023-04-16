#ifndef __CUNITTEST_CONFIG_H__
#define __CUNITTEST_CONFIG_H__

namespace UnitTest
{
#if !defined(TARGET_64BIT)
#define TARGET_64BIT
#endif

#if defined(TARGET_64BIT)
#if defined(TARGET_MAC)
    typedef unsigned long size_t;
#elif defined(TARGET_PC) && defined(COMPILER_MSVC)
    typedef unsigned __int64 size_t;
#else
    typedef unsigned long long size_t;
#endif
#elif defined(TARGET_32BIT)
    typedef unsigned int size_t;
#endif

    class TestAllocator
    {
    public:
        virtual ~TestAllocator() {}

        virtual void*        Allocate(unsigned int size, unsigned int alignment) = 0;
        virtual unsigned int Deallocate(void* ptr)                               = 0;
    };

    class TestAllocatorEx : public TestAllocator
    {
        TestAllocator* mAllocator;

    public:
        TestAllocatorEx(TestAllocator* allocator)
            : mAllocator(allocator)
            , mNumAllocations(0)
        {
        }

        virtual void* Allocate(unsigned int size, unsigned int alignment)
        {
            IncNumAllocations();
            return mAllocator->Allocate(size, alignment);
        }
        
        virtual unsigned int Deallocate(void* ptr)
        {
            DecNumAllocations();
            return mAllocator->Deallocate(ptr);
        }

        void ResetNumAllocations() { mNumAllocations = 0; }
        void IncNumAllocations()  { ++mNumAllocations; }
        void DecNumAllocations() { --mNumAllocations; }
        int  GetNumAllocations() const { return mNumAllocations; }
        int  mNumAllocations;
    };

    class TestObserver
    {
    public:
        virtual ~TestObserver() {}

        virtual void BeginSuite(const char* filename, const char* suite_name) = 0;
        virtual void EndSuite()                                               = 0;

        virtual void BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) = 0;
        virtual void EndFixture()                                                                         = 0;

        virtual void BeginTest(const char* filename, const char* suite_name, const char* fixture_name, const char* test_name) = 0;
        virtual void EndTest() = 0;
    };

    class TestContext
    {
    public:
        TestContext();

        TestAllocator* mAllocator;
        TestObserver*  mObserver;
    };

    class NullAllocator : public TestAllocator
    {
    public:
        NullAllocator() {}

        virtual void*        Allocate(unsigned int size, unsigned int alignment) { return 0; }
        virtual unsigned int Deallocate(void* ptr) { return 0; }
        void                 Release() {}
    };

    class NullObserver : public TestObserver
    {
    public:
        virtual void BeginSuite(const char* filename, const char* suite_name) {}
        virtual void EndSuite() {}

        virtual void BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) {}
        virtual void EndFixture() {}

        virtual void BeginTest(const char* filename, const char* suite_name, const char* fixture_name, const char* test_name) {}
        virtual void EndTest() {}
    };
} // namespace UnitTest

#endif ///< __CUNITTEST_CONFIG_H__
