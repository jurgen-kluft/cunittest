#ifndef __CUNITTEST_CONFIG_H__
#define __CUNITTEST_CONFIG_H__

namespace UnitTest
{
    class TestAllocator
    {
    public:
        virtual ~TestAllocator() {}

        virtual void* Allocate(unsigned int size, unsigned int alignment = sizeof(void*)) = 0;
        virtual void  Deallocate(void* ptr, int* status = nullptr)                        = 0;
    };

    class MemCheckAllocator : public TestAllocator
    {
    public:
        virtual void* Allocate(unsigned int size, unsigned int alignment = sizeof(void*));
        virtual void  Deallocate(void* ptr, int* status = nullptr);
    };

    class TestAllocatorEx : public TestAllocator
    {
        TestAllocator* mAllocator;

    public:
        TestAllocatorEx(TestAllocator* allocator)
            : mAllocator(allocator)
            , mNumAllocations(0)
            , mNumAllocationCorruptions(0)
        {
        }

        virtual void* Allocate(unsigned int size, unsigned int alignment)
        {
            IncNumAllocations();
            return mAllocator->Allocate(size, alignment);
        }

        virtual void Deallocate(void* ptr, int* _status = nullptr)
        {
            DecNumAllocations();

            int status = 0;
            mAllocator->Deallocate(ptr, &status);
            if (_status)
            {
                *_status = status;
            }
            if (status == 0)
            {
                return;
            }
            IncNumAllocationCorruptions();
        }

        void ResetEx()
        {
            mNumAllocations           = 0;
            mNumAllocationCorruptions = 0;
        }
        void IncNumAllocations() { ++mNumAllocations; }
        void DecNumAllocations() { --mNumAllocations; }
        int  GetNumAllocations() const { return mNumAllocations; }

        void IncNumAllocationCorruptions() { ++mNumAllocationCorruptions; }
        int  GetNumAllocationCorruptions() const { return mNumAllocationCorruptions; }

        int mNumAllocations;
        int mNumAllocationCorruptions;
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
        virtual void EndTest()                                                                                                = 0;
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

        virtual void* Allocate(unsigned int size, unsigned int alignment) { return 0; }
        virtual void  Deallocate(void* ptr, int* status) {}
        void          Release() {}
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

#define UNITTEST_ALLOCATOR                                                                                           \
    class fixture_ccore_alloc_t : public ncore::alloc_t                                                              \
    {                                                                                                                \
        UnitTest::TestAllocator** mAllocator;                                                                        \
                                                                                                                     \
    public:                                                                                                          \
        fixture_ccore_alloc_t(UnitTest::TestAllocator** allocator)                                                   \
            : mAllocator(allocator)                                                                                  \
        {                                                                                                            \
        }                                                                                                            \
        virtual void* v_allocate(ncore::u32 size, ncore::u32 align) { return (*mAllocator)->Allocate(size, align); } \
        virtual void  v_deallocate(void* p) { (*mAllocator)->Deallocate(p); }                                        \
    };                                                                                                               \
    static fixture_ccore_alloc_t TestAlloc(&FixtureAllocator);                                                       \
    static alloc_t*              Allocator = &TestAlloc

} // namespace UnitTest

#endif ///< __CUNITTEST_CONFIG_H__
