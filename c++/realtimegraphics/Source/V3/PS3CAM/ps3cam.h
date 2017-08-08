#ifndef PS3Cam_H
#define PS3Cam_H
#include "../CLEyeMulticam.h"
#include <boost/thread/thread.hpp>

class PS3Cam
{
public:
	PS3Cam();
	~PS3Cam();
	bool Init();
	void Run();
	const unsigned char*	GetFrame();
private:
	bool initalised;
	bool running;
	static int NumOfActiveCams;
	CLEyeCameraInstance cam;
	unsigned char*		pic;
	boost::shared_ptr<boost::thread>		camThread;
};

#endif