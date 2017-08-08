#ifndef FireWorkEffect_H
#define FireWorkEffect_H
#include "BaseEffect.h"
#include <d3d9.h>
#include <d3dx9.h>

class FireWorkEffectData : public BaseEffectData
{
public:
	FireWorkEffectData():pos(0,0,0){};

	D3DXVECTOR3 pos;
	D3DXVECTOR3 velosity;
	float TimeTillBoom;
	D3DXVECTOR3 Colour;
};

class FireWorkEffect : public BaseEffect
{
public:
	FireWorkEffect			(							);
	~FireWorkEffect			(							);
	virtual void Initalise		(BaseEffectData* EffectData	);
	virtual void Update			(float TimeStep				);
	virtual void KillMe			(							);
	virtual bool UpdateParticle	(Particle *particle, float TimeStep	);
	virtual void RenderParticle	(Particle *particle			);
private:
	virtual void CreateParticle	(Particle *particle			);
	void Reset					(							);
	D3DXVECTOR3 pos;
	D3DXVECTOR3 velosity;
	float TimeTillBoom;
	D3DXVECTOR3 Colour;
};

#endif