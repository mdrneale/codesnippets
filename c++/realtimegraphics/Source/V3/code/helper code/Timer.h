#ifndef Timer_h
#define Timer_h


class Timer
{
public:
	 Timer();
	~Timer();

	bool	Check();
	bool	Check(float &TimeStep);
	void	Reset();

	float	Duration;
	long	CurrentTime;
	long	LastTime;


};

#endif