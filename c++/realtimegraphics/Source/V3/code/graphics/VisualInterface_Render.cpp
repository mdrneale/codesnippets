#include "VisualInterface.h"


bool VisualInterface::UsingHeadTracking		( float xyz[3]	)
{
//	static D3DXVECTOR3 test[8];
//	static D3DXVECTOR3 BigChange(0,0,0);
//	D3DXVECTOR3		   NewPos(0,0,0);
//	static D3DXVECTOR3 OldPos = NewPos;
//	ht.GetPos(&NewPos.x);
//	test[7]=test[6];
//	test[6]=test[5];
//	test[5]=test[4];
//	test[4]=test[3];
//	test[3]=test[2];
//	test[2]=test[1];
//	test[1]=test[0];
//	test[0]=NewPos;
//	D3DXVECTOR3 np=(test[0]+test[1]+test[2]+test[3]+test[4]+test[5]+test[6]+test[7])/8.0f;
////	
////	D3DXVECTOR3			SmoothPos	= (NewPos+OldPos+OldPos)*0.333f;
//////SmoothPos.z = 1.70;
////	D3DXVECTOR3			DistVec		= (SmoothPos-OldPos);
////
////	float				Dist		= (DistVec.x*DistVec.x)+(DistVec.y*DistVec.y)+(DistVec.z*DistVec.z);
////
////	if (Dist>0.05f)
////	{
////		D3DXVECTOR3			DistVec2 = (BigChange-OldPos);
////		float				Dist2    = (DistVec2.x*DistVec2.x)+(DistVec2.y*DistVec2.y)+(DistVec2.z*DistVec2.z);
////		if ( (abs(Dist-Dist2) <= 0.02f) && abs(BigChange.z)<3.0f)
////		{
////			xyz[0] = BigChange.x;
////			xyz[1] = BigChange.y;
////			xyz[2] = BigChange.z;
////			OldPos = BigChange;
////			BigChange = SmoothPos;
////		}
////		else
////		{
////			xyz[0] = OldPos.x;
////			xyz[1] = OldPos.y;
////			xyz[2] = OldPos.z;
////			if (abs(SmoothPos.z)<3.0f)
////			{
////				BigChange = SmoothPos;
////			}
////		}
////
////	}
////	else
////	{
//		xyz[0] = np.x;
//		xyz[1] = np.y;
//		xyz[2] = 2.0f;//np.z;
////		OldPos = SmoothPos;
////	}

	return false;
}

void VisualInterface::Update()
{
	for (int a=0; a < (int)drawMethods.size(); a++)
	{
		drawMethods[a]->RenderScene(&drawMethodOutput);
	}
}
static float Debug_eyedist	= 0.080f;
void VisualInterface::Set3Dcam()
{
	int curcam = scene.CurrentCamera;
	scene.Cameras[0]->FOV		= scene.Cameras[1]->FOV			= scene.Cameras[curcam]->FOV;
	scene.Cameras[0]->Interest	= scene.Cameras[1]->Interest	= scene.Cameras[curcam]->Interest;
	scene.Cameras[0]->Up		= scene.Cameras[1]->Up			= scene.Cameras[curcam]->Up;
	D3DXVECTOR3 pos	= scene.Cameras[curcam]->Position;
	D3DXVECTOR3 forward = scene.Cameras[curcam]->Interest;
	D3DXVec3Normalize(&forward,&forward);
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right,&forward,&scene.Cameras[curcam]->Up);
	scene.Cameras[0]->Position	= scene.Cameras[curcam]->Position+(right*Debug_eyedist);
	scene.Cameras[1]->Position	= scene.Cameras[curcam]->Position-(right*Debug_eyedist);
}

void VisualInterface::SetBackgroundColour	( const BYTE red, const BYTE green,	const BYTE blue	) 
{
	r=red; 
	g=green; 
	b=blue;
}	

void VisualInterface::SetProjection			( float FOV, float nearPlane, float farPlane		)
{
	D3DXMATRIX	matProjection;									//projection matrix (NEW)
	const float	AspectRatio=(float)WIDTH / (float)HEIGHT;		//view port ratio (NEW)

	D3DXMatrixPerspectiveFovLH(	&matProjection, FOV, AspectRatio, nearPlane, farPlane);
	gD3dDevice->SetTransform(D3DTS_PROJECTION,&matProjection);
}

void VisualInterface::Render					(													)
{
	HRESULT hr = gD3dDevice->Present(0,0,0,0);
}
