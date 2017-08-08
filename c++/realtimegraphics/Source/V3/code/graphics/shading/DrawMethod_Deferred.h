#ifndef DrawMethod_deferred_H
#define DrawMethod_deferred_H
#include "..\mesh\mesh.h"
#include "DrawMethod.h"
#include "..\..\helper code\Timer.h"

enum SelectRenderTarget
{
	SRT_Output,
	SRT_Position,
	SRT_Normal,
	SRT_Colour,
	SRT_Ambient,
	SRT_ShadowMap,
	SRT_ShadowMapToScene,
	SRT_BlurShadowMapToScene,
	SRT_ShadeTheScene,
	SRT_DrawLightBulbs,
	SRT_ToneMap,
	SRT_NumberOfSRTs
};

class DrawMethod_Deferred: public DrawMethod
{
public:
	DrawMethod_Deferred(LPDIRECT3DDEVICE9 device,SceneData *sd);
	~DrawMethod_Deferred();
	virtual void	RenderScene(DM_Output* out);
private:
	void CreateGBuffer();
	void AddAmbient();
	void ShadeScene();
		bool GetVisability(Light* l);
		void CreateShadowMap(Light* l);
		void RenderShadowMapToScene(Light* l);
		void BlurForSoftShadows();
		void ShadeTheScene(Light* l);
		void DrawShadeSpecificObjects();
			void DrawAShadeSpecificObject(LPD3DXEFFECT Effect,Model* model);
		void DrawLightBulbs();
			void SetScreenFacingLight(const Light &l,LPD3DXEFFECT E);
	void ToneMap();
		void GetLuminance(LPDIRECT3DSURFACE9 lum);
		void DrawToneMap();
		void TVNoise();

	Mesh					ScreenQuad;

	LPDIRECT3DTEXTURE9		PositionTexture;
	LPDIRECT3DSURFACE9		PositionSurface;

	LPDIRECT3DTEXTURE9		NormalTexture;
	LPDIRECT3DSURFACE9		NormalSurface;

	LPDIRECT3DTEXTURE9		ColourTexture;
	LPDIRECT3DSURFACE9		ColourSurface;

	LPDIRECT3DTEXTURE9		SwitchTexture1;
	LPDIRECT3DSURFACE9		SwitchSurface1;

	LPDIRECT3DTEXTURE9		SwitchTexture2;
	LPDIRECT3DSURFACE9		SwitchSurface2;

	LPDIRECT3DTEXTURE9		SwitchTexture3;
	LPDIRECT3DSURFACE9		SwitchSurface3;

	LPDIRECT3DTEXTURE9		ShadowMapTexture;
	LPDIRECT3DSURFACE9		ShadowMapSurface;

	LPDIRECT3DTEXTURE9		ShadowMapDepthTexture;
	LPDIRECT3DSURFACE9		ShadowMapDepthSurface;

	LPDIRECT3DTEXTURE9		TempShadowTexture;
	LPDIRECT3DSURFACE9		TempShadowSurface;

	LPDIRECT3DTEXTURE9		LuminanceTexture;
	LPDIRECT3DSURFACE9		LuminanceSurface;

	LPD3DXEFFECT			UberEffect;

	DM_Output*				output;
	SelectRenderTarget		SRT;
	int						LightTextureID;

	//////////////////////////////////////////////////////////////////////////
	// Tone Mapping stuff
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
	Timer				T;

};

#endif
