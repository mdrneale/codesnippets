#include "FireWorkEffect.h"
#include "EffectsSystem.h"

FireWorkEffect::FireWorkEffect			(							)
{

}

FireWorkEffect::~FireWorkEffect			(							)
{

}
float notrandom()
{
	static int a=0;
	a++;
	return fmod((float)(10.24-(2*((a+4.02)+a*a))+1.05*a*2.0f),20.03f);
}
void FireWorkEffect::Initalise		(BaseEffectData* EffectData	)
{
	FireWorkEffectData* fwed = (FireWorkEffectData*)EffectData;
	pos			= fwed->pos;
	velosity	= fwed->velosity;
	Colour		= fwed->Colour;
	TimeTillBoom= fwed->TimeTillBoom;
}

static float drag=0.33f;
void FireWorkEffect::Update			(float TimeStep				)
{
	TimeTillBoom-=TimeStep;
	if (TimeTillBoom<=0.0f)
	{
		for (int a=0; a<100; a++)
		{
			CreateParticle(EffectsSystem::I()->GetParticle(this));
		}
		Reset();
	}
	pos+=velosity*TimeStep;
	velosity*=max(1.0f-(drag*TimeStep),0.1f);
	velosity.y-=(9.814f*TimeStep);
	EffectsSystem::I()->DrawScreenFacingQuad(ADD,0,pos,D3DXVECTOR2(0.3f,0.3f),Colour);
}

void FireWorkEffect::Reset()
{
	pos = D3DXVECTOR3(-0,0,-0);
	velosity.x = notrandom();
	velosity.y = 30.0f;
	velosity.z = notrandom();
	TimeTillBoom=3.0f+fmod(notrandom(),2.0f);
	Colour		= D3DXVECTOR3(abs(fmod(notrandom(),1.0f)),abs(fmod(notrandom(),1.0f)),abs(fmod(notrandom(),1.0f)));
}
void FireWorkEffect::KillMe			(							)
{
	BaseEffect::KillMe();
}

bool FireWorkEffect::UpdateParticle	(Particle *particle, float TimeStep	)
{
	particle->time-=TimeStep;
	if (particle->time<0.0f)
	{
		return false;
	}
	particle->pos+=particle->dir*TimeStep;
	particle->dir*=max(1.0f-(drag*TimeStep),0.1f);
	particle->dir.y-=(9.814f*TimeStep);
	return true;
}

void FireWorkEffect::RenderParticle	(Particle *particle			)
{
	EffectsSystem::I()->DrawScreenFacingQuad(ADD,0,particle->pos,particle->scale,D3DXVECTOR3(abs(fmod(notrandom(),1.0f)),abs(fmod(notrandom(),1.0f)),abs(fmod(notrandom(),1.0f))));
}

static float pt=2.0f;
void FireWorkEffect::CreateParticle	(Particle *particle			)
{
	particle->pos=pos;
	particle->dir.x=notrandom();
	particle->dir.y=abs(notrandom());
	particle->dir.z=notrandom();
	particle->scale.x=particle->scale.y=abs(fmod(notrandom(),0.3f));
	particle->time=pt;
}