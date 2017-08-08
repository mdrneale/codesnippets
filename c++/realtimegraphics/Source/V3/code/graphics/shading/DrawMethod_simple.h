#ifndef DrawMethod_simple_H
#define DrawMethod_simple_H

#include "DrawMethod.h"

class DrawMethod_simple: public DrawMethod
{
public:
	DrawMethod_simple(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene();
	LPDIRECT3DTEXTURE9		scene;
	LPD3DXEFFECT	simpleEffect;
};

#endif
