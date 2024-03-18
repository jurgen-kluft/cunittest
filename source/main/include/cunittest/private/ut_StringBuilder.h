#ifndef __CUNITTEST_STRINGBUILDER_H__
#define __CUNITTEST_STRINGBUILDER_H__

namespace UnitTest
{
    class TestAllocator;

    class StringBuilder
    {
    public:
        StringBuilder(TestAllocator* allocator, const int capacity = 480);
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
            GROW_SIZE   = 256
        };
        int getCapacity() const;

    private:
        void  ensureSize(int ensure);

        TestAllocator* mAllocator;
        int            mCapacity;
        int            mCursor;
        char*          mBuffer;
    };
} // namespace UnitTest

#endif ///< __CUNITTEST_STRINGBUILDER_H__
