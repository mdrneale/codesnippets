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
	SRT_NumberOfSRTs
};

class DrawMethod_ChinesePainting: public DrawMethod
{
public:
	DrawMethod_ChinesePainting(LPDIRECT3DDEVICE9 device,SceneData *sd);
	~DrawMethod_ChinesePainting();
	virtual void	RenderScene(DM_Output* out);
private:
	void CreateGBuffer();

	Mesh					ScreenQuad;

	LPDIRECT3DTEXTURE9		PositionTexture;
	LPDIRECT3DSURFACE9		PositionSurface;

	LPDIRECT3DTEXTURE9		NormalTexture;
	LPDIRECT3DSURFACE9		NormalSurface;

	LPDIRECT3DTEXTURE9		ColourTexture;
	LPDIRECT3DSURFACE9		ColourSurface;

	LPD3DXEFFECT			UberEffect;

	DM_Output*				output;
	SelectRenderTarget		SRT;

};

#endif
