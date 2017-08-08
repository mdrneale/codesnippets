#ifndef DrawMethod_DiffusePass_H
#define DrawMethod_DiffusePass_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"

class DrawMethod_DiffusePass: public DrawMethod
{
public:
	DrawMethod_DiffusePass					(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void		RenderScene			(DM_Output* out);
	void				SetPositionTexture	(LPDIRECT3DTEXTURE9 pos){PositionPass	= pos;};
	void				SetNormalTexture	(LPDIRECT3DTEXTURE9 nor){NormalPass		= nor;};
private:

	void				Render				(int a);
	void				RenderShadowMap		(int a);
	void				RenderBlur			();
	void				RenderShadows		(int a);
	void				RenderSecondShadowPass();
	Mesh				ScreenQuad;

	LPDIRECT3DTEXTURE9	PositionPass;
	LPDIRECT3DTEXTURE9	NormalPass;

	LPDIRECT3DSURFACE9	ShadowSurface;
	LPDIRECT3DTEXTURE9	ShadowPass;

	LPDIRECT3DSURFACE9	FinalShadowSurface;
	LPDIRECT3DTEXTURE9	FinalShadowPass;

	LPD3DXEFFECT		DiffuseEffect;
	LPD3DXEFFECT		ShadowEffect;
	LPD3DXEFFECT		ShadowSecondPassEffect;
	LPD3DXEFFECT		BlurEffect;
};

#endif
