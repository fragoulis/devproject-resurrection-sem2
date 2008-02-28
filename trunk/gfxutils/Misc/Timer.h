#ifndef __Timer_h__
#define __Timer_h__

#ifndef _WIN32
    #include <sys/time.h>
#else
    #include <windows.h>
	#include <Mmsystem.h>
#endif 

#include <vector>

namespace CKLib {

/*
	SimpleTimer (not used anymore in this project)
	Keeps track of time
*/
class SimpleTimer
{
    private :
        double init_time;
		double _frequency_denom;
        double counter_time;
        
    public :
		SimpleTimer(){InitTime();}
        double curtime;
		double framediff;
        void InitTime()    //initialises the app time
        {
#ifndef _WIN32
            timeval t;
            gettimeofday(&t,NULL);
            init_time = t.tv_sec + (double)(0.000001*t.tv_usec);
#else
            LARGE_INTEGER finishfr, frequency;   
			QueryPerformanceFrequency(&frequency);
			_frequency_denom = 1.0 / (double)frequency.QuadPart;
			
			QueryPerformanceCounter(&finishfr);
			curtime = init_time = (double)(finishfr.QuadPart * _frequency_denom);
#endif            
        }
        double GetTime()    //gets elapsed time in seconds
        {		
#ifndef _WIN32
            timeval t;
            gettimeofday(&t,NULL);
            curtime = (double)(t.tv_sec + 0.000001*t.tv_usec) - init_time;

#else 
			LARGE_INTEGER finishfr;
			QueryPerformanceCounter(&finishfr);
			curtime = (double)(finishfr.QuadPart * _frequency_denom) - init_time;
            //curtime = (timeGetTime() - (float)init_time)*0.001f;
#endif            
			return curtime;
        }
     
        inline void start()
        {
            counter_time = GetTime();
        }
        inline double stop()
        {
            return GetTime() - counter_time;
        }
};

/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/
/*						NEW TIMER CLASSES												*/
/****************************************************************************************/
/****************************************************************************************/

#define MY_TIME_SAMPLES 10

class MegaTimer;
/*
	Timer : 
	class that functions as a simple counter, by starting & stopping()
*/
class Timer
{
	double _startTime;
	double _localCurTime;
	double _scale;

	friend class MegaTimer;

public :
	// ctor
	Timer();

	// utils
	void start();
	double stop();

	// accessors - mutators
	void setScale(const double scale);
	double scale() const;
};

/*
	Timesource : 
	A generalization of a timer source.
	Returns data in form of time intervals from last call
*/
class TimeSource
{
protected :
	double _firstTimeGotten;		
public :
	TimeSource();
	virtual ~TimeSource(){}
	virtual double getTime() = 0;
};

/*
	ClockTimesource : 
	A time source based on system clock
*/
class ClockTimeSource : public TimeSource
{
	double _prevTime; 

public :
	ClockTimeSource();
	virtual double getTime();	// return normal clock time
};

/*
	FileTimesource : 
	A time source based on file data
*/
class FileTimeSource : public TimeSource
{
	double	* _timesBuffer;
	unsigned  _timesStored;
	unsigned  _timesIndex;

public :
	FileTimeSource(const std::string& timeFile);
	~FileTimeSource();
	virtual double getTime();	// return normal clock time
};

/*
	MegaTimer : 
	Singleton class that manages every timer & timesource in the app
	can save to file specified interval data
	could scale all the app's timers
*/
class MegaTimer
{
	TimeSource *	   _timeSource;
	double			   _curTime;
	double			   _startTime;		// for saving / loading

	std::vector<Timer *> _timerList;
	double			   _prevTimes[MY_TIME_SAMPLES];
	unsigned		   _timeSampleCounter;

	bool			   _hasStarted;

	std::vector<double> _saveBuffer;
	bool				_recording;
	double				_recStartTime;

	static MegaTimer *	_instance;

public : 
	MegaTimer(); // starts with clock
	void setSource(TimeSource * source);	// could be file, clock for ""
	void startRecord();
	void stopRecord(const std::string& filename);
	void addTimer(Timer * timer);
	double getFrameTime();
	void update();
	void reset();

	static void init();
	static void destroy();
	static MegaTimer * instance();
	static MegaTimer * fast_instance();

};

#include <Misc/Timer.inl>

}

#endif
