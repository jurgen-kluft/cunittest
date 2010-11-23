#ifndef __X_CORE_UNIT_TEST_TIMEHELPERS_H__
#define __X_CORE_UNIT_TEST_TIMEHELPERS_H__

namespace UnitTest
{
	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Module:
	//     UnitTest
	// Summary:
	//     The timer used to time unit test suites, fixtures and tests.
	// Description:
	//      This class is the Timer class used to time every single unit test as well
	//      as the total time
	// See Also:
	//     TimeHelpers::sleepMs
	//------------------------------------------------------------------------------
	class Timer
	{
	public:
					Timer();
		
		void		start();
		int			getTimeInMs() const;

	private:
		int			mData[8];
	};


	namespace TimeHelpers
	{
		//------------------------------------------------------------------------------
		// Author:
		//     Jurgen Kluft
		// Module:
		//     UnitTest
		// Summary:
		//     The timer used to time unit test suites, fixtures and tests.
		// Description:
		//      This function
		// See Also:
		//     Timer
		//------------------------------------------------------------------------------
		void	sleepMs(int ms);
	}

}


#endif	///< __X_CORE_UNIT_TEST_TIMEHELPERS_H__