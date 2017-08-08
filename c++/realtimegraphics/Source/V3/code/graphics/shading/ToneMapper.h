#ifndef ToneMapper_H
#define ToneMapper_H
#include <d3d9.h>
#include <d3dx9.h>
#include "..\..\helper code\Timer.h"
#include "..\SceneData.h"

class ToneMapper
{
public:
	ToneMapper();
	void	RenderScene(LPDIRECT3DTEXTURE9 Luminance,DM_Output* outputRender);
private:
	float	GetLuminance(LPDIRECT3DTEXTURE9 Luminance);
	void	Render(DM_Output* outputRender);
	float	LuminanceToReach;
	float	CurrentLuminance;
	float	VisibleLightSize;
	float	MaximumLuminanceLevel;
	float	MinimumLuminanceLevel;
	float	LightAdjustSpeed;
	Timer	UpdateLuminanceTimer;
	Timer	UpdateCurrentLuminanceTimer;
};

#endif