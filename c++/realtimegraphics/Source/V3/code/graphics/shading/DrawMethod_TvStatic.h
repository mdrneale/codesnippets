#ifndef DrawMethod_TvStatic_H
#define DrawMethod_TvStatic_H
#include "..\mesh\mesh.h"
#include "..\..\helper code\Timer.h"
#include "DrawMethod.h"

class DrawMethod_TvStatic: public DrawMethod
{
public:
	DrawMethod_TvStatic(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	LPDIRECT3DTEXTURE9		preprocessRender;
private:
	void					TvStatic();
	Mesh					ScreenQuad;
	int						tvStaticTextureID;
	int						currentFrame;
	float					tvStaticWeight;
	Timer					T;
	LPD3DXEFFECT			TVStaticEffect;
};

#endif
