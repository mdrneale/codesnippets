#ifndef camera_h
#define camera_h
#include <d3dx9.h>

class Camera
{
public:
	Camera(): FOV(D3DX_PI / 4.0f),Position(0,0,0),Up(0,1,0),Interest(1,0,0){};
	D3DXVECTOR3	 Position;
	D3DXVECTOR3	 Up;
	D3DXVECTOR3	 Interest;
	void MoveForward(float distance);
	void MoveRight(float distance);
	void Rotate(float x,float y);
	D3DXMATRIX	 GetRotation();
	D3DXMATRIX		test;
	float	FOV;

};

#endif
