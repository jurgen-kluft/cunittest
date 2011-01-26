#ifndef __XUNITTEST_CONFIG_H__
#define __XUNITTEST_CONFIG_H__

namespace UnitTest
{
	extern void*	Allocate(int size);
	extern void		Deallocate(void* ptr);

	extern void		BeginFixture(const char* filename, const char* suite_name, const char* fixture_name);
	extern void		EndFixture();
}


#endif	///< __XUNITTEST_CONFIG_H__
