#include "TestEffect.h"
#include "EffectsSystem.h"

TestEffect::TestEffect			(							)
{

}

TestEffect::~TestEffect			(							)
{

}

void TestEffect::Initalise		(BaseEffectData* EffectData	)
{
	TestEffectData* ted = (TestEffectData*)EffectData;
	pos[0]=ted->pos[0];
	pos[1]=ted->pos[1];
	pos[2]=ted->pos[2];
}

void TestEffect::Update			(float TimeStep				)
{
	static float t=0.0f;
	static float t2=0.0f;
	static float add=0.5f;
	t+=TimeStep;
	t2+=TimeStep*3.0f;
	pos[0]=2.0f*sin(t2);
	pos[2]=2.0f*cos(t2);
	if (t>add)
	{
		Particle *p = EffectsSystem::I()->GetParticle(this);
		CreateParticle(p);
		t=0.0f;
	}
}

void TestEffect::KillMe			(							)
{
	BaseEffect::KillMe();
}

bool TestEffect::UpdateParticle	(Particle *particle, float TimeStep	)
{
	particle->time+=TimeStep;
	particle->pos[1]-=9.8f*TimeStep;

	if (particle->pos[1]<=0.0f)
	{
		particle->pos[1]=0.0f;
	}
	particle->scale[0]=1-(particle->time/8.0f);
	particle->scale[1]=1-(particle->time/8.0f);
	if (particle->time>=8.0f)
	{
		return false;
	}
	return true;
}

void TestEffect::RenderParticle	(Particle *particle			)
{
	EffectsSystem::I()->DrawScreenFacingQuad(ADD,0,particle->pos);
}

void TestEffect::CreateParticle	(Particle *particle			)
{
	particle->pos[0]=pos[0];
	particle->pos[1]=4;
	particle->pos[2]=pos[2];
	particle->scale[0]=0.5f;
	particle->scale[1]=0.5f;
	particle->time=0.0f;
}