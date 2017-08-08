#include "Timer.h"
#include <time.h>

Timer::Timer()
{
	Reset();
}

Timer::~Timer()
{

}

bool	Timer::Check()
{
	CurrentTime	=	clock();
	if ( CurrentTime > ( LastTime + (Duration * 1000.0f) ) )
	{
		LastTime	=	clock();
		return true;
	}
	return false;
}

bool	Timer::Check(float &TimeStep)
{
	CurrentTime	=	clock();
	if ( CurrentTime > ( LastTime + (Duration * 1000.0f) ) )
	{
		TimeStep	=	(CurrentTime - LastTime) * 0.001f;
		LastTime	=	clock();
		return true;
	}
	return false;
}

void	Timer::Reset()
{
	CurrentTime	=	clock();
	LastTime	=	clock();
	Duration	=	1.0f;
}
