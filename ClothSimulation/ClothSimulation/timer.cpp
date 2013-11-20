#include <windows.h>
#include <assert.h>
#include "timer.h"

namespace timing
{

	Timer::Timer()
	{
		init();
	}

	Timer::Timer( const float fps )
	{
		init();
		fps_ = fps;

	}

	void Timer::setFPS( float fps )
	{
		fps_ = fps;
	}

	void Timer::setAlarm( Clocks sleep_clocks )
	{
		wake_after_ = getCurrentClocks() + sleep_clocks;
	}

	void Timer::setAlarm()
	{
		assert( fps_ );

		//	set alarm based on current fps
		wake_after_ =  getCurrentClocks() + 
			getFixedClocksPerSec() /
			static_cast< Clocks >( fps_ );
	}

	float Timer::getIdealFramesPerSec() const
	{
		return fps_;
	}

	timing::Clocks Timer::getIdealClocksPerFrame() const
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );

		return static_cast< Clocks >( static_cast< float >( 
			freq.QuadPart ) * ( 1.0f / fps_ ) );
	}

	bool Timer::shouldWake() const
	{
		return wake_after_ <= getCurrentClocks();
	}

	void Timer::startStopwatch()
	{
		stopwatch_start_ = getCurrentClocks();
	}

	timing::Clocks Timer::getStopwatchClocks() const
	{
		return getCurrentClocks() - stopwatch_start_;
	}

	timing::Clocks Timer::getFixedClocksPerSec() const
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		
		return freq.QuadPart;
	}

	void Timer::init()
	{
		fps_ = 0;
		wake_after_ = 0;
		stopwatch_start_ = getCurrentClocks();
	}

	float Timer::getFPS() const
	{
		return fps_;
	}

	float Timer::getStopWatchSec() const
	{
		float sec = static_cast< float >( getCurrentClocks() - 
			stopwatch_start_ ) / static_cast< float >( getFixedClocksPerSec() );
		//assert( sec );
		return sec;
	}

	timing::Clocks Timer::getCurrentClocks() const
	{
		LARGE_INTEGER clocks;
		QueryPerformanceCounter( &clocks );

		return clocks.QuadPart;
	}

}	//	end timing