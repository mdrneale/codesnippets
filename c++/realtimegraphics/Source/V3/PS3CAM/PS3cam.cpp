#include "PS3cam.h"

int PS3Cam::NumOfActiveCams = 0;

PS3Cam::PS3Cam()
{
	initalised = false;
}

PS3Cam::~PS3Cam()
{
	running = false;
	if (initalised)
	{
		camThread->join();
		CLEyeCameraStop(cam);
		CLEyeDestroyCamera(cam);
		delete pic;
	}
}

bool PS3Cam::Init()
{
	initalised = false;
	int numCams = CLEyeGetCameraCount();
	if (numCams <= NumOfActiveCams)
	{
		return 0;
	}
	cam = NULL;
	cam = CLEyeCreateCamera( CLEyeGetCameraUUID(NumOfActiveCams) , CLEYE_GRAYSCALE, CLEYE_QVGA, 60);
	if (!cam)
	{
		return 0;
	}
	if (!CLEyeCameraStart(cam))
	{
		return 0;
	}
	++NumOfActiveCams;
	pic			= new unsigned char[2*320*240];
	camThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&PS3Cam::Run,this)));
	return true;
}

void PS3Cam::Run()
{
	initalised = true;
	running = true;
	while(running)
	{
		if (cam && pic)
		{
			CLEyeCameraGetFrame(cam,pic,2000);
		}
	}
}

const unsigned char* PS3Cam::GetFrame()
{
	return pic;
}
