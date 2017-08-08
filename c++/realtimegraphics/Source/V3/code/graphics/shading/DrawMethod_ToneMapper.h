#ifndef DrawMethod_ToneMapper_H
#define DrawMethod_ToneMapper_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"
#include "..\..\helper code\Timer.h"

class DrawMethod_ToneMapper: public DrawMethod
{
public:
	DrawMethod_ToneMapper					(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void		RenderScene			(DM_Output* out);
	void				SetLuminanceSurface	(LPDIRECT3DSURFACE9 lum,LPDIRECT3DSURFACE9 pic) {Luminance	= lum;LPic=pic;};
	void				SetSceneTexture		(LPDIRECT3DTEXTURE9 pic) {ScenePic = pic;};

private:
	void				GetLuminance		(LPDIRECT3DSURFACE9 Luminance);
	void				Render				(DM_Output* outputRender);
	float				LuminanceToReach;
	float				MaxSceneLuminance;
	float				MinSceneLuminance;
	float				CurrentLuminance;
	float				VisibleLightSize;
	float				MaximumLuminanceLevel;
	float				MinimumLuminanceLevel;
	float				LightAdjustSpeed;
	Timer				UpdateLuminanceTimer;
	Timer				UpdateCurrentLuminanceTimer;
	LPDIRECT3DSURFACE9	Luminance;
	LPDIRECT3DTEXTURE9	ScenePic;

	LPDIRECT3DSURFACE9	LPic;
	LPDIRECT3DSURFACE9	ToSurf;
	LPDIRECT3DTEXTURE9	Totex;
	LPD3DXEFFECT		ToneMapEffect;
	Mesh				ScreenQuad;

};

#endif