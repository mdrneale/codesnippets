#ifndef DrawMethod_AmbientOcclusion_H
#define DrawMethod_AmbientOcclusion_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"

class DrawMethod_AmbientOcclusion: public DrawMethod
{
public:
	DrawMethod_AmbientOcclusion(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	void SetPositionTexture	( const LPDIRECT3DTEXTURE9 NewPos){PositionPass	= NewPos;};
	void SetNormalTexture	( const LPDIRECT3DTEXTURE9 NewNor){NormalPass	= NewNor;};
private:
	void RenderAO();
	void RenderBlur();
	Mesh					ScreenQuad;
	LPD3DXEFFECT			AmbientOcclusionEffect;
	LPD3DXEFFECT			BlurEffect;
	LPDIRECT3DTEXTURE9		AOPass;
	LPDIRECT3DSURFACE9		AOSurface;

	LPDIRECT3DTEXTURE9		PositionPass;
	LPDIRECT3DTEXTURE9		NormalPass;
};

#endif
