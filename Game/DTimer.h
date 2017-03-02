#ifndef _DTIMER_H
#define _DTIMER_H

#define MACOSX

#ifdef WIN32
#include <windows.h>
#endif

#ifdef MACOSX
#include <sys/time.h>
#endif

#include  "definitions.h"

BEGIN_NAMESPACE_DIRAC2D
	
	class DTimer
	{
	public:
		
		void start();
		
		void stop();
		
		void tick();
		
		dfloat value();
		
		void setCurrentTime(dfloat time);
		
		double getCurrentTime();

		static double getTime();
				
		dfloat getIncreasingDuration();
		
		dfloat getDecreasingDuration();
		
		void setIncreasingDuration(dfloat);
		
		void setDecreasingDuration(dfloat);
		
		DTimer();
		
		DTimer(unsigned , dfloat);
		
		
	private:
		
#ifdef WIN32
		dfloat fps;
		dfloat currentTime , startTime;
		dfloat diffTime;
#endif
		
#ifdef MACOSX
		unsigned int fps;
		double currentTime , startTime;
		double diffTime;
#endif
		
		unsigned loopCount;
		dfloat increasingDuration;
		dfloat decreasingDuration;
		
		void initTimer();
		
#ifdef MACOSX
		double m_start_sec;
		double m_start_msec;
#endif
		
	};

END_NAMESPACE_DIRAC2D

#endif