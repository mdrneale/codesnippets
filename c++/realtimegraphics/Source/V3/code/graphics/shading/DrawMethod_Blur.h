#ifndef DrawMethod_Blur_H
#define DrawMethod_Blur_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"
#include "..\..\helper code\Timer.h"

class DrawMethod_Blur: public DrawMethod
{
public:
	DrawMethod_Blur							(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void		RenderScene			(DM_Output* out);
	void				SetBlurTexture		(LPDIRECT3DTEXTURE9 pic) {BlurTexture = pic;};
	void				SetTempBlureSurface	(LPDIRECT3DSURFACE9 lum,LPDIRECT3DTEXTURE9 pic) {TempBlurSurface= lum;TempBlurTexture = pic;};

private:
	void				Render				(DM_Output* outputRender);

	LPDIRECT3DSURFACE9	TempBlurSurface;
	LPDIRECT3DTEXTURE9	TempBlurTexture;
	LPDIRECT3DTEXTURE9	BlurTexture;

	LPD3DXEFFECT		BlurEffect;
	Mesh				ScreenQuad;

};

#endif