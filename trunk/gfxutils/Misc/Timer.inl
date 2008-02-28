// Timer Class 

inline Timer :: Timer()
:_scale(1.0f)
{
	MegaTimer::instance()->addTimer(const_cast<Timer *>(this));
	_startTime = MegaTimer::instance()->getFrameTime();
}

inline void Timer :: start()
{
	_startTime = MegaTimer::fast_instance()->getFrameTime();
}

inline double Timer :: stop()
{
	return _scale*(MegaTimer::fast_instance()->getFrameTime() - _startTime);
}


// MegaTimer Class

inline double MegaTimer :: getFrameTime()
{
	return _curTime - _startTime;
}