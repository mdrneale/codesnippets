#ifndef DrawMethod_deferred2_H
#define DrawMethod_deferred2_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"
#include "DrawMethod_Shadows.h"
#include "DrawMethod_SpecificShading.h"
#include "DrawMethod_AmbientPass.h"
#include "DrawMethod_TvStatic.h"
#include "DrawMethod_EffectsPass.h"
#include "DrawMethod_ToneMapper.h"

class DrawMethod_deferred2: public DrawMethod
{
public:
	DrawMethod_deferred2(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
private:

	void Render();
	void LightPass();
	void SetScreenFacingLight(const Light &l);
	void RenderLightBulbs();
	DrawMethod_AmbientPass		Ap;
	DrawMethod_Shadows			Sp;
	DrawMethod_SpecificShading	SSp;
	DrawMethod_TvStatic			TV;
	DrawMethod_EffectsPass		EP;

	DrawMethod_ToneMapper		TM;

	Mesh					ScreenQuad;
	Mesh					LightQuad;
	LPDIRECT3DTEXTURE9		PositionPass;
	LPDIRECT3DSURFACE9		PositionSurface;

	LPDIRECT3DTEXTURE9		NormalPass;
	LPDIRECT3DSURFACE9		NormalSurface;

	LPDIRECT3DTEXTURE9		ColourPass;
	LPDIRECT3DSURFACE9		ColourSurface;

	LPDIRECT3DTEXTURE9		SwitchPass;
	LPDIRECT3DSURFACE9		SwitchSurface;

	LPDIRECT3DTEXTURE9		TempSwitchPass1;
	LPDIRECT3DSURFACE9		TempSwitchSurface1;

	LPDIRECT3DTEXTURE9		TempSwitchPass2;
	LPDIRECT3DSURFACE9		TempSwitchSurface2;

	LPDIRECT3DTEXTURE9		mipmaplum;
	LPDIRECT3DSURFACE9		mipmaplumSurface;

	LPDIRECT3DTEXTURE9		LowResTempPass;
	LPDIRECT3DSURFACE9		LowResTempSurface;

	LPD3DXEFFECT			DeferredEffect;

	unsigned int			LightTextureID;

};

#endif
