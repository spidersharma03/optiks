#include "DTimer.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D
	
	//dfloat DTimer::currentTime = 0.0f;
	//dfloat DTimer::startTime = 0.0f;
	//dfloat DTimer::diffTime = 0.0f;
	//dfloat DTimer::increasingDuration = 0.0f;
	//dfloat DTimer::decreasingDuration = 0.0f;
#ifdef WIN32
	static LARGE_INTEGER HighPerformanceFreq;
	static BOOL HighPerformanceTimerSupport = FALSE;
#endif
#ifdef MACOSX
#endif
	
	DTimer::DTimer()
	{
		initTimer();
	}
	
	DTimer::DTimer(unsigned loopCount , dfloat increasingDuration)
	{
		initTimer();
		DTimer::increasingDuration = increasingDuration;
	}
	
	void DTimer::initTimer()
	{
		diffTime = 0.0f;
		decreasingDuration = 0.0f;
		
		increasingDuration = 1000;
		
#ifdef WIN32
		startTime = GetTickCount();
		HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
#endif
		
#ifdef MACOSX
		timeval t;
		gettimeofday(&t, 0);
		m_start_sec = t.tv_sec;
		m_start_msec = t.tv_usec * 0.001f;
		startTime = (double)t.tv_sec * 1000 + (double)t.tv_usec * 0.001f;
#endif
		currentTime = startTime;
	}
	
	void DTimer::tick()
	{
#ifdef WIN32
		LARGE_INTEGER nTime;
		
		if ( HighPerformanceTimerSupport )
		{
			BOOL queriedOK = QueryPerformanceCounter(&nTime);
			
			currentTime = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);
			
		}
		else
		{
			currentTime = GetTickCount();
		}
#endif
		
#ifdef MACOSX
		timeval t;
		gettimeofday(&t, 0);
		currentTime = (double)t.tv_sec * 1000 + (double)t.tv_usec * 0.001f;;	
#endif
		static long count = 0;
		count++;
		if ( diffTime > increasingDuration )
		{
			fps = count*1000.0/diffTime;
#ifdef WIN32
			printf("\nFPS = %f" , fps);
#endif
#ifdef MACOSX
			printf("\nFPS = %d" , fps);
#endif
			count = 0;
			startTime = currentTime;
		}
		diffTime = ( currentTime - startTime );
	}
	
	void DTimer::setCurrentTime(dfloat time)
	{
		startTime = time;
	}
	
	double DTimer::getCurrentTime()
	{
		double time;
#ifdef WIN32
		LARGE_INTEGER nTime;
		
		if ( HighPerformanceTimerSupport )
		{
			BOOL queriedOK = QueryPerformanceCounter(&nTime);
			
			time = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);
		}
		else
		{
			time = GetTickCount();
		}
#endif
		
#ifdef MACOSX
		timeval t;
		gettimeofday(&t, 0);
		time =  (double)t.tv_sec  * 1000 + (double)t.tv_usec * 0.001f;
#endif
		//printf("Time = %f\n", time);
		return time;
	}

    double DTimer::getTime()
    {
        double time;
    #ifdef WIN32
        LARGE_INTEGER nTime;
        
        if ( HighPerformanceTimerSupport )
        {
            BOOL queriedOK = QueryPerformanceCounter(&nTime);
            
            time = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);
        }
        else
        {
            time = GetTickCount();
        }
    #endif
        
    #ifdef MACOSX
        timeval t;
        gettimeofday(&t, 0);
        time =  (double)t.tv_sec  * 1000 + (double)t.tv_usec * 0.001f;
    #endif
        //printf("Time = %f\n", time);
        return time;
    }
	void DTimer::stop()
	{
		
	}
	
	dfloat DTimer::value()
	{
		return diffTime / increasingDuration;
	}
	
	void DTimer::setIncreasingDuration(dfloat increasingDuration)
	{
		DTimer::increasingDuration = increasingDuration;
	}
	
	dfloat DTimer::getIncreasingDuration()
	{
		return increasingDuration;
	}
	
	void DTimer::setDecreasingDuration(dfloat decreasingDuration)
	{
		DTimer::decreasingDuration = decreasingDuration;
	}
	
	dfloat DTimer::getDecreasingDuration()
	{
		return decreasingDuration;
	}

END_NAMESPACE_DIRAC2D
	