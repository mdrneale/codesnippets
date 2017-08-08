#ifndef DrawMethod_Light_H
#define DrawMethod_Light_H
#include "DrawMethod.h"

class DrawMethod_Light: public DrawMethod
{
public:
	DrawMethod_Light(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	LPD3DXEFFECT	LightEffect;

private:
	void RenderLightPass(DM_Output* out);
	LPDIRECT3DTEXTURE9		blendPass2Texture;
	LPDIRECT3DSURFACE9		blendPass2Surface;
	Mesh					ScreenQuad;
};

#endif
