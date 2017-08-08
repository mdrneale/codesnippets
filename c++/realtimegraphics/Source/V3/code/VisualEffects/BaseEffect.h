#ifndef BaseEffect_H
#define BaseEffect_H
#include <d3d9.h>
#include <d3dx9.h>

class BaseEffectData
{
};
class BaseEffect;
class Particle
{
public:
	Particle():Active(false),Effect(0),time(0){};
	bool Active;
	BaseEffect* Effect;

	D3DXVECTOR3 pos;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 dir;
	float time;
};

class BaseEffect
{
public:
	BaseEffect					(							){};
	~BaseEffect					(							){};
	virtual void Initalise		(BaseEffectData* EffectData	)=0;
	virtual void Update			(float TimeStep				)=0;
	virtual void KillMe			(							){Active=false;};
	virtual bool UpdateParticle	(Particle *particle, float TimeStep	)=0;
	virtual void RenderParticle	(Particle *particle			)=0;
	bool IsActive				(							){return Active;};
private:
	virtual void CreateParticle	(Particle *particle			)=0;
	bool Active;
};

#endif