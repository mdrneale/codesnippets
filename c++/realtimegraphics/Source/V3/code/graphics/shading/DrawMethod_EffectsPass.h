#ifndef DrawMethod_EffectsPass_H
#define DrawMethod_EffectsPass_H
#include "DrawMethod.h"

class DrawMethod_EffectsPass: public DrawMethod
{
public:
	DrawMethod_EffectsPass					(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void		RenderScene			(DM_Output* out);
private:
	void Render();
	void GetRenderInfo();

	LPD3DXEFFECT		EffectEffect;
};

#endif
