#ifndef DrawMephod_BlendOpperations_H
#define DrawMephod_BlendOpperations_H
#include "..\mesh\mesh.h"
#include "DrawMethod.h"

class DrawMephod_BlendOpperations: public DrawMethod
{
public:
	DrawMephod_BlendOpperations(){};
	DrawMephod_BlendOpperations(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void			RenderScene(DM_Output* out);
	LPDIRECT3DTEXTURE9		Pass1;
	LPDIRECT3DTEXTURE9		Pass2;
	unsigned int			Opp;
private:
	void					Blend();
	D3DXHANDLE				GetBlendMode();
	LPDIRECT3DDEVICE9		d3DDevice;
	SceneData			*	sceneData;
	Mesh					ScreenQuad;
	LPD3DXEFFECT			BlendEffect;
};

#endif