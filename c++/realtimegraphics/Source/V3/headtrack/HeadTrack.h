#ifndef HeadTrack_H
#define HeadTrack_H
#include <boost/thread/thread.hpp>
#include "../PS3CAM/PS3cam.h"

class HeadTracker
{
public:
	HeadTracker();
	~HeadTracker();
	bool Init();
	void GetPos(float* Pos);
private:
	void Run();

	const unsigned char * frame;
	const unsigned char * frame2;
	bool running;
	bool initalised;
	PS3Cam test;
	PS3Cam test2;
	boost::shared_ptr<boost::thread>		HeadTrackerThread;
	float x,y,z;
};

#endif