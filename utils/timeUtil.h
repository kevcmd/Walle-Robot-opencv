//Timer Class adapted from https://gist.github.com/mcleary
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>
#ifndef TIME_UTIL_L
#define TIME_UTIL_L

#define TIMER_MIN_SECS 15
#define TIMER_MAX_SECS 45
// TIME_UTIL

class TimeUtil
{
public:
	TimeUtil()
	{
		randomTimer(TIMER_MIN_SECS, TIMER_MAX_SECS);
	}
	
	void randomTimer(int timerMin, int timerMax)
	{
		timer = timerMin + (rand() % (timerMax - timerMin + 1));
	}
	
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }
    
    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }
    
    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;
        
        if(m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }
        
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }
    
    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

    bool getTimer(int tTime = 0)
	{
		if(!m_bRunning)
			start();
		if(tTime != 0 && timer != tTime)
			timer = tTime;
		if(elapsedSeconds() < timer)
			return true;
		else
		{
			stop();
			return false;
		}
	}
	
	bool getMilliTimer(int tTime)
	{
		if(!m_bRunning)
			start();
		if(tTime != 0 && timer != tTime)
			timer = tTime;
		if(elapsedMilliseconds() >= timer)
		{
			stop();
			return false;
		}
		else
			return true;
	}
	
	void resetTimer(bool randomTime)
	{
		stop();
		if(randomTime)
			randomTimer(TIMER_MIN_SECS, TIMER_MAX_SECS);
		else
			timer = 0;
	}

private:
	int timer;
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};
#endif
