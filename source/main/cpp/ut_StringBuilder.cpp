#include "xunittest/private/ut_Config.h"
#include "xunittest/private/ut_StringBuilder.h"
#include "xunittest/private/ut_Utils.h"

namespace UnitTest
{
	StringBuilder::StringBuilder(const int capacity)
		: mDefaultSize(STATIC_CHUNK_SIZE)
		, mCapacity(0)
		, mBuffer(0)
	{
		mDefaultBuffer[0] = '\0';
		growBuffer(capacity);
	}

	StringBuilder::~StringBuilder()
	{
		if (mBuffer != mDefaultBuffer)
			__private::GetAllocator()->Deallocate(mBuffer);
	}

	char const* StringBuilder::getText() const
	{
		return mBuffer;
	}

	StringBuilder& StringBuilder::operator << (const char* txt)
	{
		int const streamLength = gStringLength(mBuffer);
		int const txtLength = gStringLength(txt);

		int const bytesLeft = mCapacity - streamLength;
		int const bytesRequired = (int)txtLength + 1;

		if (bytesRequired > bytesLeft)
		{
			int const requiredCapacity = bytesRequired + mCapacity - bytesLeft;
			growBuffer(requiredCapacity);
		}

		for (int i=0; i<(txtLength+1); i++)
		{
			mBuffer[streamLength + i] = txt[i];
		}

		return *this;
	}
	StringBuilder& StringBuilder::operator << (const void* p)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%0X", (size_t)p);
		*this << dest;
		return *this;    
	}
	StringBuilder& StringBuilder::operator << (char const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}

	StringBuilder& StringBuilder::operator << (short const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (int const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (long long const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (long const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (unsigned char const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (unsigned short const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (unsigned int const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (unsigned long long const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (unsigned long const n)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%i", n);
		*this << dest;
		return *this;
	}
	StringBuilder& StringBuilder::operator << (float const f)
	{
		char dest[256];
		gStringPrint(dest, sizeof(dest), "%f", f);
		*this << dest;
		return *this;    
	}
	StringBuilder& StringBuilder::operator << (double const d)
	{
		char dest[256];
		float f = (float)d;
		gStringPrint(dest, sizeof(dest), "%f", f);
		*this << dest;
		return *this;    
	}

	int StringBuilder::getCapacity() const
	{
		return mCapacity;
	}

	void StringBuilder::growBuffer(int desiredCapacity)
	{
		int const newCapacity = gRoundUpToMultipleOfPow2Number(desiredCapacity, 32);
		if (newCapacity <= mDefaultSize)
		{
			char* buffer = mDefaultBuffer;
			if (mBuffer)
			{
				if (mDefaultBuffer != mBuffer)
					gStringCopy(buffer, mBuffer, mCapacity);
			}
			else
			{
				gStringCopy(buffer, "", mCapacity);
			}

			if (mBuffer != mDefaultBuffer && mBuffer!=0)
			{
				__private::GetAllocator()->Deallocate(mBuffer);
			}

			mBuffer = buffer;
			mCapacity = newCapacity;
			return;
		}

		char* buffer = (char*)__private::GetAllocator()->Allocate(newCapacity);
		if (mBuffer)
			gStringCopy(buffer, mBuffer, mCapacity);
		else
			gStringCopy(buffer, "", mCapacity);

		if (mBuffer != mDefaultBuffer)
		{
			if (mBuffer!=0)
				__private::GetAllocator()->Deallocate(mBuffer);
		}

		mBuffer = buffer;
		mCapacity = newCapacity;
	}

}
