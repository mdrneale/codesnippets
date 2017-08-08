#ifndef DrawMethod_AmbientPass_H
#define DrawMethod_AmbientPass_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"

class DrawMethod_AmbientPass: public DrawMethod
{
public:
	DrawMethod_AmbientPass					(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void		RenderScene			(DM_Output* out);
	void				SetPositionTexture	(LPDIRECT3DTEXTURE9 pos){PositionPass	= pos;};
	void				SetNormalTexture	(LPDIRECT3DTEXTURE9 nor){NormalPass		= nor;};
	void				SetColourTexture	(LPDIRECT3DTEXTURE9 col){ColourPass		= col;};
private:

	void				Render				();
	Mesh				ScreenQuad;

	LPDIRECT3DTEXTURE9	PositionPass;
	LPDIRECT3DTEXTURE9	NormalPass;
	LPDIRECT3DTEXTURE9	ColourPass;

	LPD3DXEFFECT		AmbientColourEffect;
};

#endif
