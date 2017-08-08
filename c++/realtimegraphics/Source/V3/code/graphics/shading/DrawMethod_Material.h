#ifndef DrawMethod_Material_H
#define DrawMethod_Material_H
#include "DrawMethod.h"

class DrawMethod_Material: public DrawMethod
{
public:
	DrawMethod_Material(LPDIRECT3DDEVICE9 device,SceneData *sd);
	virtual void	RenderScene(DM_Output* out);
	LPD3DXEFFECT			MaterialEffect;
private:
	void RenderMaterialPass();
	void SetWorldMatrix			( const Model &mod				);
	void SetVeiwMatrix			( const Camera &cam				);
	void SetVeiwMatrix			( const Light &l				);
	void SetProjectionMatrix	( float FOV, float nearPlane, float farPlane);
};

#endif
