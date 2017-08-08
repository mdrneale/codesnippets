#ifndef DrawMethod_SpecificShading_H
#define DrawMethod_SpecificShading_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"

class DrawMethod_SpecificShading: public DrawMethod
{
public:
	DrawMethod_SpecificShading(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	void			SetPositionTexture(LPDIRECT3DTEXTURE9 pos)	{PositionPass	= pos;};
	void			SetNormalTexture(LPDIRECT3DTEXTURE9 norm)	{NormalPass		= norm;};
	void			SetColourTexture(LPDIRECT3DTEXTURE9 col)	{ColourPass		= col;};
	void			SetShadowTexture(LPDIRECT3DTEXTURE9 shad)	{ShadowPass		= shad;};

private:

	void Render	(LPD3DXEFFECT Effect,DM_Output* out,Model* model);

	Mesh					ScreenQuad;
	LPDIRECT3DTEXTURE9		PositionPass;
	LPDIRECT3DTEXTURE9		NormalPass;
	LPDIRECT3DTEXTURE9		ColourPass;
	LPDIRECT3DTEXTURE9		ShadowPass;
};

#endif
