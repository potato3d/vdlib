#ifndef _VR_RANDOM_H_
#define _VR_RANDOM_H_

#include <vr/common.h>

namespace vr {

/*!
	Pseudo-Random Number Generator based on the Mersenne Twister.
 */
class VRBASE_EXPORT Random
{
public:
	//! Initializes the PRNG using a DWORD.
	static void seed( uint32 s );

	//! Initializes the PRNG using an array of DWORDs. 
	static void seed( uint32 *array, uint32 size );

	/*!
		If the PRNG has not been sown yet, seed it automatically using high-entropy
		data sources. If the PRNG has already been sown and you want to re-seed it,
		pass force = true.

		\return true if the auto-seeder has sown the PRNG.
				false if the PRNG had already been sown (and force = false).
	 */
	static bool autoSeed( bool force = false );

	//! Generates a random uint32 in the interval [0,0xFFFFFFFF].
	static uint32 integer32();

	//! Generates a random int32 in the interval [0,0x7FFFFFFF] (always positive).
	inline static int32 integer31()
	{
		return static_cast<int32>( integer32() >> 1 );
	}

	//! Generates a random real number in the interval [0,1] (32 bit resolution).
	inline static double realInIn()
	{
		// divided by 2^32-1
		return ( integer32() * ( 1.0 / 4294967295.0 ) );
	}

	//! Generates a random real number in the interval [0,1) (32 bit resolution).
	inline static double realInOut()
	{
		// divided by 2^32
		return ( integer32() * ( 1.0 / 4294967296.0 ) );
	}

	//! Generates a random real number in the interval (0,1) (32 bit resolution).
	inline static double realOutOut()
	{
		// divided by 2^32
		return ( ( integer32() + 0.5 ) * ( 1.0 / 4294967296.0 ) ); 
	}

	//! Generates a random real number in the interval [0,1) (53 bit resolution).
	inline static double realHiRes()
	{
		uint32 a = ( integer32() >> 5 );
		uint32 b = ( integer32() >> 6 );
		return ( a * 67108864.0 + b ) * ( 1.0 / 9007199254740992.0 );
	}

	inline static double real( double min, double max )
	{
		return ( min + ( max - min ) * realInIn() );
	}

private:
	// whether either seed() or autoSeed() has been called
	static bool sm_sown;

	// period parameters:
	enum { N = 624 };
	enum { M = 397 };

	// internal RNG state:
	static uint32 sm_state[N];	// the array for the state vector
	static int sm_index;		// index == N+1 means state[N] is not initialized
};

} // namespace vr

#endif
