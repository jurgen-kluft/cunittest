#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_StringBuilder.h"
#include "cunittest/private/ut_Utils.h"
#include "cunittest/private/ut_Types.h"

namespace UnitTest
{
    StringBuilder::StringBuilder(TestAllocator* allocator, const int capacity)
        : mAllocator(allocator)
        , mCapacity(0)
        , mCursor(0)
        , mBuffer(0)
    {
        ensureSize(capacity);
    }

    StringBuilder::~StringBuilder() { mAllocator->Deallocate(mBuffer); }

    char const* StringBuilder::getText() const { return mBuffer; }

    void StringBuilder::ensureSize(int ensure)
    {
        if (ensure > mCapacity)
        {
            int newCapacity = mCapacity + GROW_SIZE;
            if (newCapacity < ensure)
                newCapacity = ensure;

            if (mBuffer == nullptr)
            {
                mBuffer = (char*)mAllocator->Allocate(newCapacity + 1);
                mBuffer[0] = '\0';
            }
            else
            {
                char* newBuffer = (char*)mAllocator->Allocate(newCapacity + 1);
                gStringCopy(newBuffer, mBuffer, mCursor + 1);
                mAllocator->Deallocate(mBuffer);
                mBuffer   = newBuffer;
            }
            mCapacity = newCapacity;
        }
    }

    StringBuilder& StringBuilder::operator<<(const char* txt)
    {
        int const bytesRequired = gStringLength(txt);
        ensureSize(mCursor + bytesRequired);

        for (int i = 0; i < bytesRequired; i++)
            mBuffer[mCursor++] = txt[i];
        mBuffer[mCursor] = '\0';

        return *this;
    }
    StringBuilder& StringBuilder::operator<<(const void* p)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%0X", (u64)p);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(char const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }

    StringBuilder& StringBuilder::operator<<(short const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(int const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(long long const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(long const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(unsigned char const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(unsigned short const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(unsigned int const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(unsigned long long const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(unsigned long const n)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%i", n);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(float const f)
    {
        ensureSize(mCursor + 32);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 32, "%f", f);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }
    StringBuilder& StringBuilder::operator<<(double const d)
    {
        ensureSize(mCursor + 64);
        {
            mCursor += gStringPrint(mBuffer + mCursor, 64, "%f", d);
            mBuffer[mCursor] = '\0';
        }
        return *this;
    }

    int StringBuilder::getCapacity() const { return mCapacity; }

} // namespace UnitTest
