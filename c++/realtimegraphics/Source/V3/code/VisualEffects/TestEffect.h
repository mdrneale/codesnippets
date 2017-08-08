#ifndef TestEffect_H
#define TestEffect_H
#include "BaseEffect.h"

class TestEffectData : public BaseEffectData
{
public:
	TestEffectData(){pos[0]=0;pos[1]=5;pos[2]=0;};

	float pos[3];
};

class TestEffect : public BaseEffect
{
public:
	TestEffect			(							);
	~TestEffect			(							);
	virtual void Initalise		(BaseEffectData* EffectData	);
	virtual void Update			(float TimeStep				);
	virtual void KillMe			(							);
	virtual bool UpdateParticle	(Particle *particle, float TimeStep	);
	virtual void RenderParticle	(Particle *particle			);
private:
	virtual void CreateParticle	(Particle *particle			);
	float pos[3];
};

#endif