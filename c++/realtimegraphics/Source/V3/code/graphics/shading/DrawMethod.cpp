#include "DrawMethod.h"
#include "..\VisualInterface.h"

void DrawMethod::SetWorldMatrix			( const Model &mod, LPD3DXEFFECT effect)
{
	D3DXMATRIX	matRotationX, matRotationY,	matRotationZ, matScale,	matTranslation; 

	D3DXMatrixScaling(&matScale,mod.Scaling.x,mod.Scaling.y,mod.Scaling.z);
	D3DXMatrixRotationX(&matRotationX,mod.Rotation.x);
	D3DXMatrixRotationY(&matRotationY,mod.Rotation.y);
	D3DXMatrixRotationZ(&matRotationZ,mod.Rotation.z);
	D3DXMatrixTranslation(&matTranslation,mod.Position.x,mod.Position.y,mod.Position.z);
	D3DXMATRIX MatrixWorld = (	matScale * matRotationX * matRotationY * matRotationZ * matTranslation);

	effect->SetMatrix("WorldMatrix",&MatrixWorld);
}

void DrawMethod::SetWorldMatrix			( const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale, const D3DXMATRIX &rot, LPD3DXEFFECT effect)
{
	D3DXMATRIX	matScale,	matTranslation; 

	D3DXMatrixScaling(&matScale,scale.x,scale.y,scale.z);
	D3DXMatrixTranslation(&matTranslation,pos.x,pos.y,pos.z);
	D3DXMATRIX MatrixWorld = (	matScale * rot * matTranslation);

	effect->SetMatrix("WorldMatrix",&MatrixWorld);
}

void DrawMethod::SetWorldMatrix			( const D3DXVECTOR2 pos, const D3DXVECTOR2 scale, LPD3DXEFFECT effect)
{
	D3DXMATRIX	matRotationX, matRotationY,	matRotationZ, matScale,	matTranslation; 

	D3DXMatrixScaling(&matScale,scale.x,scale.y,1.0f);
	D3DXMatrixRotationX(&matRotationX,0);
	D3DXMatrixRotationY(&matRotationY,0);
	D3DXMatrixRotationZ(&matRotationZ,0);
	D3DXMatrixTranslation(&matTranslation,pos.x,pos.y,0.0f);
	D3DXMATRIX MatrixWorld = (	matScale * matRotationX * matRotationY * matRotationZ * matTranslation);

	effect->SetMatrix("WorldMatrix",&MatrixWorld);
}

void DrawMethod::SetVeiwMatrix			( const Camera &cam, LPD3DXEFFECT effect)
{
	D3DXMATRIX	MatrixView;	
	float xyz[3];
	if (VisualInterface::I()->UsingHeadTracking(xyz))
	{
		D3DXVECTOR3 pos,interest;
		pos = xyz;
		pos+=cam.Position;

		interest=cam.Interest;
		interest.x+=xyz[0];
		interest.y+=xyz[1];
		D3DXMatrixLookAtLH( &MatrixView, &pos, &interest, &cam.Up );
	}
	else
	{
		D3DXMatrixLookAtLH( &MatrixView, &cam.Position, &cam.Interest, &cam.Up );
	}
	effect->SetMatrix("ViewMatrix",&MatrixView);
}
void DrawMethod::SetProjectionMatrix	( float FOV, float nearPlane, float farPlane, LPD3DXEFFECT effect, float aspect)
{
	D3DXMATRIX	matProjection;									//projection matrix (NEW)
	const float	AspectRatio=aspect;		//view port ratio (NEW)
	float xyz[3];
	if (VisualInterface::I()->UsingHeadTracking(xyz))
	{
		SetProjectionMatrix(xyz[0],xyz[1],xyz[2],AspectRatio,effect);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(	&matProjection, FOV, AspectRatio, nearPlane, farPlane);
		effect->SetMatrix("ProjectionMatrix",&matProjection);
	}
}

void DrawMethod::SetProjectionMatrix	( float FOV, float nearPlane, float farPlane, LPD3DXEFFECT effect)
{
	D3DXMATRIX	matProjection;									//projection matrix (NEW)
	const float	AspectRatio=(float)WIDTH / (float)HEIGHT;		//view port ratio (NEW)
	float xyz[3];
	if (VisualInterface::I()->UsingHeadTracking(xyz))
	{
		SetProjectionMatrix(xyz[0],xyz[1],xyz[2],AspectRatio,effect);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(	&matProjection, FOV, AspectRatio, nearPlane, farPlane);
		effect->SetMatrix("ProjectionMatrix",&matProjection);
	}
}

void DrawMethod::SetProjectionMatrix	( float headX, float headY, float headDist, float AspectRatio,LPD3DXEFFECT effect)
{
	headDist=sqrt(headDist*headDist+headX*headX+headY*headY);
	D3DXMATRIX	matProjection;									//projection matrix (NEW)
	float nearPlane = 0.05f;
    D3DXMatrixPerspectiveOffCenterLH( &matProjection,   
							   nearPlane*( -0.5f * AspectRatio + headX)/headDist, 
                               nearPlane*(	0.5f * AspectRatio + headX)/headDist, 
                               nearPlane*(-0.5f - headY)/headDist, 
                               nearPlane*( 0.5f - headY)/headDist, 
                               nearPlane, 1000.0f);
	effect->SetMatrix("ProjectionMatrix",&matProjection);
}

void DrawMethod::SetVeiwMatrix			( const Light *cam, LPD3DXEFFECT effect)
{
	D3DXMATRIX	MatrixView;	

	D3DXMatrixLookAtLH( &MatrixView, &cam->Position, &cam->Interest, &cam->Up );

 	effect->SetMatrix("ViewMatrix",&MatrixView);
}