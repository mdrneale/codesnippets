#ifndef EffectData_H
#define EffectData_H

#include <d3d9.h>
#include <d3dx9.h>

struct EffectData
{
	EffectData():Effect(NULL),NeedsPosMap(false),NeedsColMap(false),NeedsNormMap(false),BlendType(0){};
	LPD3DXEFFECT	Effect;
	unsigned int	BlendType;
	bool			NeedsPosMap;
	bool			NeedsColMap;
	bool			NeedsNormMap;
};
#endif