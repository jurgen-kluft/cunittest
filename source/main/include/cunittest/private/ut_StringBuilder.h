#ifndef __CUNITTEST_MEMORYOUTSTREAM_H__
#define __CUNITTEST_MEMORYOUTSTREAM_H__

namespace UnitTest
{
    class TestAllocator;

    class StringBuilder
    {
    public:
        StringBuilder(TestAllocator* allocator, const int capacity = 256);
        ~StringBuilder();

        char const* getText() const;

        StringBuilder& operator<<(const char* txt);
        StringBuilder& operator<<(const void* p);
        StringBuilder& operator<<(char const n);
        StringBuilder& operator<<(short const n);
        StringBuilder& operator<<(int const n);
        StringBuilder& operator<<(long long const n);
        StringBuilder& operator<<(long const n);
        StringBuilder& operator<<(unsigned char const n);
        StringBuilder& operator<<(unsigned short const n);
        StringBuilder& operator<<(unsigned int const n);
        StringBuilder& operator<<(unsigned long long const n);
        StringBuilder& operator<<(unsigned long const n);
        StringBuilder& operator<<(float const f);
        StringBuilder& operator<<(double const f);

        enum ESettings
        {
            STATIC_CHUNK_SIZE = 2048,
            GROW_CHUNK_SIZE   = 32
        };
        int getCapacity() const;

    private:
        void operator=(StringBuilder const&);
        void growBuffer(int capacity);

        TestAllocator* mAllocator;
        int            mDefaultSize;
        char           mDefaultBuffer[STATIC_CHUNK_SIZE];

        int   mCapacity;
        char* mBuffer;
    };
} // namespace UnitTest

#endif ///< __CUNITTEST_MEMORYOUTSTREAM_H__
