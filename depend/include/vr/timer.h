#ifndef _VR_TIMER_H
#define _VR_TIMER_H

#include <vr/common.h>

namespace vr {

/*!
	Measures a time interval. This implementation prioritizes performance and resolution,
	but will probably run out-of-sync with real-world clocks.
 */
class VRBASE_EXPORT Timer
{
public:
	typedef uint64 Stamp;

	inline static Stamp tick();

	static const double SECS_PER_TICK;

public:
	inline Timer() : mTime( 0 )
	{;}

	inline Timer( Stamp time ) : mTime( time )
	{;}

	//! Returns the timer's reference time in seconds.
	inline double getTime() const
	{
		return ( mTime * SECS_PER_TICK );
	}

	//! Returns the timer's reference timestamp.
	inline Stamp getTimeStamp() const
	{
		return mTime;
	}

	//! Sets the timer's reference timestamp.
	inline void setTimeStamp( Stamp time )
	{
		mTime = time;
	}

	//! Returns elapsed time (in seconds) since last restart.
	inline double elapsed() const
	{
		return ( ( tick() - mTime ) * SECS_PER_TICK );
	}

	/*!
		Returns elapsed time (in seconds) from this timer's reference timestamp up to
		the specified timestamp.
	 */
	inline double elapsed( Stamp time ) const
	{
		return ( ( time - mTime ) * SECS_PER_TICK );
	}

	//! Restarts the timer and returns elapsed time (in seconds) since last restart.
	inline double restart()
	{
		Stamp previousTick = mTime;
		mTime = tick();
		return ( ( mTime - previousTick ) * SECS_PER_TICK );
	}

private:
	static double computeSecsPerTick();

private:
	Stamp mTime;
};


// tick() platform-specific implementation:
#if defined(_WIN32)
	inline Timer::Stamp Timer::tick()
	{
		// reads ts from the TSC
		volatile Stamp ts;
		volatile uint32 highDWord, lowDWord;
		_asm
		{
			xor eax, eax
			xor edx, edx
			_emit 0x0f
			_emit 0x31
			mov highDWord,edx
			mov lowDWord,eax
		}

		*reinterpret_cast<volatile uint32*>( &ts ) = lowDWord;
		*( reinterpret_cast<volatile uint32*>( &ts ) + 1 ) = highDWord;

		return ts;
	}
#elif defined(unix) || defined(__linux) || defined(__FreeBSD__) || defined(__CYGWIN__) || defined (__APPLE__) || defined (macintosh)
	#include <sys/time.h>
	inline Timer::Stamp Timer::tick()
	{
		struct timeval tv;
		gettimeofday( &tv, NULL );
		return ( static_cast<Stamp>( tv.tv_sec ) * 1000000 + static_cast<Stamp>( tv.tv_usec ) );
	}
#else
#error vr::Timer does not support your OS.
#endif

} // namespace vr

#endif
