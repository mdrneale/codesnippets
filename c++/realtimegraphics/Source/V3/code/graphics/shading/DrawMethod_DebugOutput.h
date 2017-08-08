#ifndef DrawMethod_DebugOutput_H
#define DrawMethod_DebugOutput_H
#include "DrawMethod.h"
#include "..\mesh\mesh.h"

class DrawMethod_DebugOutput: public DrawMethod
{
public:
	static DrawMethod_DebugOutput* GetDebugger(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	void			DisplayTexture(D3DXVECTOR2 ScreenPos , D3DXVECTOR2 Scale, LPDIRECT3DTEXTURE9 TextureToDisplay);
private:

	DrawMethod_DebugOutput(LPDIRECT3DDEVICE9 device,SceneData *sd);
	void			Render();
	static DrawMethod_DebugOutput*	Instance;
	Mesh							ScreenQuad;

	LPDIRECT3DTEXTURE9				displayTexture;
	LPDIRECT3DSURFACE9				displaySurface;

	LPD3DXEFFECT					DebugEffect;
};

#endif
