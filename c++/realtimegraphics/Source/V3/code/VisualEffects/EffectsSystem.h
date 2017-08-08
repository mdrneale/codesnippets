#ifndef EffectsSystem_h
#define EffectsSystem_h
#include <list>
#include "..\helper code\Timer.h"
#include "..\graphics\VisualInterface.h"
#include "..\graphics\vertex\Vertex.h"
#include "baseEffect.h"
#include <d3d9.h>
#include <d3dx9.h>

const int  NumberOfParticles	= 500;
const int  NumberOfVertices		= (500 * 4);
const int  NumberOfTextures		= 1;	

const int NumberOfBlendModes = 2;
enum BlendMode
{
	ADD,
	MULTIPLY
};

class RenderInfo
{
public:
	int							FirstTextureID;
	int							TextureIDs[NumberOfBlendModes*NumberOfTextures];
	IDirect3DVertexBuffer9		*VertexBuffer;
	IDirect3DIndexBuffer9		*IndexBuffer;
};

class EffectsSystem
{
public:
	~EffectsSystem	(												);
	static EffectsSystem*	I(										);
	bool	Initalise(												);
	void	Add		(BaseEffect* Effect,BaseEffectData* EffectData	);
	void	Update	(												);
	void	Remove	(BaseEffect* Effect								);
	void	DrawScreenFacingQuad(BlendMode bm, int TextureID , float PosScale[5]);
	void	DrawScreenFacingQuad(BlendMode bm, int TextureID , D3DXVECTOR3 Pos,D3DXVECTOR2 Scale);
	void	DrawScreenFacingQuad(BlendMode bm, int TextureID , D3DXVECTOR3 Pos,D3DXVECTOR2 Scale,D3DXVECTOR3 Colour);
	Particle* GetParticle(BaseEffect* Effect);
	RenderInfo* GetRenderInfo	();
private:
	static EffectsSystem*	Instance;
	EffectsSystem	(												);
	void	ResetRenderInfo	();
	bool	CreateVertexBuffer();
	void	ResetCounters();
	void	LoadEffectsTextures();

	void	DrawScreenFacingQuad(int firstvert,float PosScale[5]);

	// counters
	int							NumberOfQuads;
	int							NumberOfAdditiveQuads;
	//////////////////////////////////////////////////////////////////////////
	void	PrepairRender();
	void	Render();

	RenderInfo					RenderInformation;
	Vertex						Verts		[NumberOfVertices];
	IDirect3DVertexBuffer9		*VertexBuffer;
	IDirect3DIndexBuffer9		*IndexBuffer;
	Particle					Particles	[NumberOfParticles];
	std::list<BaseEffect*>		EffectsRunning;
	float						TimeStep;
	Timer						Time;
	LPD3DXEFFECT				EffectsEffect;
	LPDIRECT3DDEVICE9			device;
};
#endif