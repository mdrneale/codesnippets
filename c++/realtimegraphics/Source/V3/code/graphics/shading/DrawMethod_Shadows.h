#ifndef DrawMethod_Shadows_H
#define DrawMethod_Shadows_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"
#include "DrawMethod_Blur.h"

class DrawMethod_Shadows: public DrawMethod
{
public:
	DrawMethod_Shadows(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	void			SetPositionTexture(LPDIRECT3DTEXTURE9 pos){PositionPass=pos;};
	void			SetLowResSurface(LPDIRECT3DSURFACE9 lores,LPDIRECT3DTEXTURE9 lorestex){LowResSurface=lores;LowResTexture=lorestex;};
	void			SetNormalTexture(LPDIRECT3DTEXTURE9 norm){NormalPass=norm;};
	void			SetColourTexture(LPDIRECT3DTEXTURE9 col){ColourPass=col;};
	void			SetLightTextureID(unsigned int LightTex){LightTextureID=LightTex;};
	DrawMethod_Blur	Blur;
private:

	void CreateAShadowMap	(unsigned int LightSource);
	void RenderTheShadow	(unsigned int LightSource);
	bool GetVisability		(unsigned int LightSource);
	void DrawLight			(unsigned int LightSource);
	void ShadeScene			(unsigned int LightSource);
	void SetScreenFacingLight(const Light &l);

	Mesh					ScreenQuad;
	LPDIRECT3DTEXTURE9		PositionPass;
	LPDIRECT3DTEXTURE9		NormalPass;
	LPDIRECT3DTEXTURE9		ColourPass;

	LPDIRECT3DSURFACE9		LowResSurface;
	LPDIRECT3DTEXTURE9		LowResTexture;

	LPDIRECT3DTEXTURE9		ShadowMapTexture;
	LPDIRECT3DSURFACE9		ShadowMapSurface;

	LPDIRECT3DTEXTURE9		ShadowMapDepthTexture;
	LPDIRECT3DSURFACE9		ShadowMapDepthSurface;

	LPD3DXEFFECT			ShadowEffect;
	LPD3DXEFFECT			UberEffect;
	LPD3DXEFFECT			ShadingEffect;

	unsigned int LightTextureID;
};

#endif
