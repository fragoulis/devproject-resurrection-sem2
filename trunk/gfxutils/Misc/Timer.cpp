#include <Misc/Timer.h>
#include <Misc/defines.h>
#include <iostream>
#include <ctime>

using namespace std;
using namespace CKLib;

MegaTimer * MegaTimer :: _instance(0);

// Timer Class 

void Timer :: setScale(const double scale)
{
	_scale = scale;
}

double Timer :: scale() const
{
	return _scale;
}

// TimeSource class
TimeSource :: TimeSource()
:_firstTimeGotten(0.0f)
{	
}

// ClockTimeSource class

ClockTimeSource :: ClockTimeSource()
:_prevTime(0.0f)
{		
	_firstTimeGotten = timeGetTime()*0.001;
	_prevTime = _firstTimeGotten;
}

double ClockTimeSource :: getTime()
{
    double ctime = timeGetTime()*0.001 - _prevTime;
	_prevTime += ctime;
	return ctime;
}

// FileTimeSource class

FileTimeSource :: FileTimeSource(const std::string& timeFile)
:_timesStored(0),
_timesIndex(0)
{
	FILE * fp = fopen(timeFile.c_str(),"rb");
	ASSERT(fp);
	fread(&_timesStored,sizeof(unsigned),1,fp);
	_timesBuffer = new double[_timesStored];
	fread(_timesBuffer,sizeof(double),_timesStored,fp);
}

FileTimeSource :: ~FileTimeSource()
{
	if(_timesStored)
		delete []_timesBuffer;
}

double FileTimeSource :: getTime()
{
	return (_timesIndex < _timesStored) ? _timesBuffer[_timesIndex++] - _firstTimeGotten: -1.0;
}

// MegaTimer class

void MegaTimer :: init()
{
	_instance = new MegaTimer();
}

void MegaTimer :: destroy()
{
	if(_instance)
		delete _instance;
	timeEndPeriod(1);
}

MegaTimer * MegaTimer :: instance()
{
	if(!_instance)
	{
		_instance = new MegaTimer();
	}
	return _instance;
}

MegaTimer * MegaTimer :: fast_instance()
{
	return _instance;
}

MegaTimer :: MegaTimer()
:_hasStarted(false),
_timeSampleCounter(0),
_recording(false)
{
	timeBeginPeriod(1);
	_timeSource = new ClockTimeSource();
	_startTime = _curTime = _timeSource->getTime();
}

/* reset :
Resets the Megatimer starting offset
Start from _startTime , with new samples
*/

void MegaTimer :: reset() 
{
	_timeSampleCounter = 0;
	_curTime = _timeSource->getTime();
	_startTime = _curTime = 0.0f;
}

void MegaTimer :: update()
{
	double frametime =  _timeSource->getTime();
	if(frametime < 0)
	{
		delete _timeSource;
		_timeSource = new ClockTimeSource();	// default to clock again
		frametime = _prevTimes[MY_TIME_SAMPLES - 1];
	}
	// Normalize time
	double sum = 0;
	unsigned max_avg = MIN(unsigned(MY_TIME_SAMPLES),_timeSampleCounter);
		for(unsigned i=0;i<max_avg;++i)
			sum += _prevTimes[i];
	const double curavg = MIN(0.06f,(sum + frametime)/ (max_avg + 1.0f));
	if(max_avg < MY_TIME_SAMPLES)
		_prevTimes[max_avg] = curavg;
	else
	{
		unsigned i=0;
		for(;i<(MY_TIME_SAMPLES-1);++i)
			_prevTimes[i] = _prevTimes[i+1];
		_prevTimes[i] = curavg;
	}
	_curTime += curavg;
	++_timeSampleCounter;	

	if((_curTime - _startTime) < 0)
	{
		ASSERT(0);
	}
	if(_recording)
		_saveBuffer.push_back(curavg);
}

void MegaTimer :: setSource(TimeSource * source)
{
	_timeSource = source;
	_timeSampleCounter = 0;
}

void MegaTimer :: addTimer(Timer * timer)
{
	_timerList.push_back(timer);
}

void MegaTimer :: startRecord()
{
	if(_recording)
		return;
	_recording = true;
}

void MegaTimer :: stopRecord(const std::string& filename)
{
	if(!_recording)
		return;
	FILE * fp = fopen(filename.c_str(),"wb");
	ASSERT(fp);
	const unsigned bufsize = unsigned(_saveBuffer.size());
	fwrite(&bufsize,sizeof(unsigned),1,fp);				// Write the number of stored moves
	fwrite(&(_saveBuffer[0]),sizeof(double),bufsize,fp);// Write the time data (w/ zero offset)
	fclose(fp);
	_recording = false;
	_saveBuffer.clear();
}
