#include "HeadTrack.h"

HeadTracker::HeadTracker()
{

}

HeadTracker::~HeadTracker()
{
	running = false;
	if (initalised)
	{
		HeadTrackerThread->join();
	}
}

bool HeadTracker::Init()
{
	initalised = false;
	if (!test.Init())
	{
 		return false;
	}
	if (!test2.Init())
	{
 		return false;
	}
	HeadTrackerThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&HeadTracker::Run,this)));
	return true;
}

void HeadTracker::GetPos(float* Pos)
{
	Pos[0] = x;
	Pos[1] = y;
	Pos[2] = z;
}

void HeadTracker::Run()
{
	initalised = true;
	running = true;
	int x1=0;
	int y1=0;
	int x2=0;
	int y2=0;
	int dx=0;
	int dy=0;
	int posx=0;
	int posy=0;
	float pxls=0.0f;
	bool found1st;
	bool found2nd;

	x=y=z=0.0f;

	while(running)
	{
		frame = test.GetFrame();
		frame2= test2.GetFrame();
		found1st = false;
		found2nd = false;
		for (int a=0; a<320*240; a++)
		{
			if (!found1st)
			{
				if (frame[a]>10)
				{
					x1=a%320;
					y1=a/320;
					found1st = true;
				}
			}
			if (!found2nd)
			{
				if (frame2[a]>10)
				{
					x2=a%320;
					y2=a/320;
					found2nd = true;
				}
			}
			if (found1st && found2nd)
			{
				break;
			}
		}
		if (found1st && found2nd)
		{
			dx	= abs(x1-x2);
			dy	= abs(y1-y2);
			pxls= sqrt((float)dx*dx+dy*dy);
			posx= ((x1+x2)/2)-160;
			posy= ((y1+y2)/2)-120;
			float nx,ny,nz;
			nz	= 35.70f / pxls;
			nx	= (-0.39f * (posx)*nz) / 100.0f;
			ny	= (-0.39f * (posy)*nz) / 100.0f;
			x=(nx+x)*0.5f;
			y=(ny+y)*0.5f;
			z=(nz+z)*0.5f;
		}
	}
}
