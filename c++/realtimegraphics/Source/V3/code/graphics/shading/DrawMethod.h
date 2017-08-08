#ifndef DrawMethod_H
#define DrawMethod_H
#include <d3d9.h>
#include <d3dx9.h>
#include "..\SceneData.h"
#include "DM_Output.h"


class DrawMethod
{
public:
		DrawMethod(){};
		DrawMethod(LPDIRECT3DDEVICE9 device,SceneData *sd) : d3DDevice(device),sceneData(sd){};
		virtual void	RenderScene(DM_Output* out)=0;
//private:
		void SetWorldMatrix		( const Model &mod, LPD3DXEFFECT effect);
		void SetWorldMatrix		( const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale, const D3DXMATRIX &rot, LPD3DXEFFECT effect);
		void SetWorldMatrix		( const D3DXVECTOR2 pos, const D3DXVECTOR2 scale, LPD3DXEFFECT effect);
		void SetVeiwMatrix		( const Camera &cam, LPD3DXEFFECT effect);
		void SetVeiwMatrix		( const Light *cam, LPD3DXEFFECT effect);
		void SetProjectionMatrix( float FOV, float nearPlane, float farPlane, LPD3DXEFFECT effect);
		void SetProjectionMatrix( float headX, float headY, float headDist, float AspectRatio,LPD3DXEFFECT effect);
		void SetProjectionMatrix( float FOV, float nearPlane, float farPlane, LPD3DXEFFECT effect, float aspect);

public:
		LPDIRECT3DDEVICE9		d3DDevice;
		SceneData			*	sceneData;
		LPDIRECT3DTEXTURE9		scene;
};

#endif