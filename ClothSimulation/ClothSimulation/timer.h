#pragma once

namespace timing
{
	typedef __int64 Clocks;

	class Timer
	{

	private:

		//	variables
		float fps_;

		Clocks wake_after_;

		Clocks stopwatch_start_;

	public:
		
		//	ctors
		Timer();
		
		Timer( const float fps );

		//	functions
		void setFPS( float fps );

		float getFPS() const;

		void startStopwatch();		//	used by clients to record time intervals; 
									//	mostly used for computing FPS
		Clocks getStopwatchClocks() const;

		float getStopWatchSec() const;

		void setAlarm( Clocks sleep_clocks );

		void setAlarm();

		bool shouldWake() const;

		float getIdealFramesPerSec() const;
		
		Clocks getIdealClocksPerFrame() const;
		
		Clocks Timer::getFixedClocksPerSec() const;
		
		Clocks getCurrentClocks() const;

	private:
		
		void Timer::init();

	};	//	end Timer

}	//	end timing