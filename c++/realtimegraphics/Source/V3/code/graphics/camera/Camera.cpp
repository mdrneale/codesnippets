#include "Camera.h"

void Camera::MoveForward(float distance)
{
	D3DXVECTOR3 forwardvec = (Interest-Position);
	D3DXVec3Normalize(&forwardvec,&forwardvec);
	Position+=(forwardvec*distance);
	Interest+=(forwardvec*distance);
}

void Camera::MoveRight(float distance)
{
	D3DXVECTOR3 forwardvec = (Interest-Position);
	D3DXVec3Normalize(&forwardvec,&forwardvec);
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right,&forwardvec,&Up);
	Position+=(right*distance);
	Interest+=(right*distance);
}

D3DXMATRIX	 Camera::GetRotation()
{
	return test;

	//D3DXVECTOR3 vDirection,vRotAxis;

	//D3DXVec3Normalize(&vDirection,&(Interest - Position)); 
	//D3DXVec3Cross(&vRotAxis,&vDirection,&Up); //strafe vector
	//D3DXVec3Normalize(&vRotAxis,&vRotAxis);
	//float ty = vRotAxis.y;
	//vRotAxis.y = vRotAxis.z;
	//vRotAxis.z = ty;
	//return vRotAxis * D3DX_PI;
}

void Camera::Rotate(float x,float y)
{
	D3DXVECTOR3 vDirection,vRotAxis;
	D3DXMATRIX matRotAxis,matRotZ;

	D3DXVec3Normalize(&vDirection,
					  &(Interest - Position)); //create direction vector

	D3DXVec3Cross(&vRotAxis,&vDirection,&Up); //strafe vector
	D3DXVec3Normalize(&vRotAxis,&vRotAxis);

	//create rotation matrices
	D3DXMatrixRotationAxis(&matRotAxis,
						   &vRotAxis,
						   y);

	D3DXMatrixRotationY(&matRotZ,x);
	//rotate direction
	D3DXVec3TransformCoord(&vDirection,&vDirection,&(matRotAxis * matRotZ));
	//rotate up vector
	D3DXVec3TransformCoord(&Up,&Up,&(matRotAxis * matRotZ));
	//translate up vector
	
	D3DXVec3Normalize(&Up,&Up);
	D3DXVec3Normalize(&vDirection,&vDirection);

	test = (matRotAxis * matRotZ);
	Interest = vDirection + Position;
}

//
//Vector3 Camera::GetForward()
//{
//	Vector3 t(Interest.x-Position.x,Interest.y-Position.y,Interest.z-Position.z);
//	float a=1.0f/(t.x+t.y+t.z);
//	t.x*=a;
//	t.y*=a;
//	t.z*=a;
//	return t;
//}
//
//Vector3 Camera::GetUp()
//{
//	return Up;
//}
//
//Vector3 Camera::GetLeft()
//{
//	return Up;
//}