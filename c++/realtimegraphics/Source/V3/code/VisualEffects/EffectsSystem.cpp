#include "EffectsSystem.h"

EffectsSystem *	EffectsSystem::Instance = NULL;

EffectsSystem *	EffectsSystem::I()
{
	if ( !Instance )
	{
		Instance = new EffectsSystem();
	}
	return Instance;
}

EffectsSystem::EffectsSystem	(												)
{
	Time.Reset();
	Time.Duration = 0.02f;
	TimeStep = 0.0f;
	NumberOfQuads=0;
}

EffectsSystem::~EffectsSystem	(												)
{

}

bool	EffectsSystem::Initalise(												)
{
	device = VisualInterface::I()->Naughty();
	LoadEffectsTextures();
	return CreateVertexBuffer();
}

RenderInfo* EffectsSystem::GetRenderInfo	()
{
	return (&RenderInformation);
}

void	EffectsSystem::ResetRenderInfo	()
{
	RenderInformation.VertexBuffer	= VertexBuffer;
	RenderInformation.IndexBuffer	= IndexBuffer;
	for (int a = 0; a < NumberOfBlendModes*NumberOfTextures; a++)
	{
		RenderInformation.TextureIDs[a]	= 0;
	}
}

void	EffectsSystem::LoadEffectsTextures()
{
	RenderInformation.FirstTextureID = VisualInterface::I()->LoadTexture("textures\\light.tga");
}

Particle* EffectsSystem::GetParticle(BaseEffect* Effect)
{
	for (int a = 0; a < NumberOfParticles; a++)
	{
		if (!Particles[a].Active)
		{
			Particles[a].Active = true;
			Particles[a].Effect = Effect;
			return &Particles[a];
		}
	}
	return 0;
}

void	EffectsSystem::Add		(BaseEffect* Effect,BaseEffectData* EffectData	)
{
	Effect->Initalise(EffectData);
	EffectsRunning.push_back(Effect);
}

void	EffectsSystem::Update	(												)
{
	if (Time.Check(TimeStep))
	{
		ResetCounters();
		ResetRenderInfo();
		std::list<BaseEffect*>::iterator Effect;
		for ( Effect=EffectsRunning.begin() ; Effect != EffectsRunning.end(); Effect++ )
		{
			if ((*Effect)->IsActive())
			{
				(*Effect)->Update(TimeStep);
			}
			else
			{
				EffectsRunning.erase(Effect);
			}
		}
		for (int a = 0; a < NumberOfParticles; a++)
		{
			if (Particles[a].Active && Particles[a].Effect->IsActive())//add change for particles active on an inactive effect
			{
				if (!Particles[a].Effect->UpdateParticle(&Particles[a],TimeStep))
				{
					Particles[a].Active		= false;
					Particles[a].time		= 0.0f;
					Particles[a].pos[0]		= 0.0f;
					Particles[a].pos[1]		= 0.0f;
					Particles[a].pos[2]		= 0.0f;
					Particles[a].scale[0]	= 1.0f;
					Particles[a].scale[1]	= 1.0f;
					Particles[a].dir[0]		= 0.0f;
					Particles[a].dir[1]		= 0.0f;
					Particles[a].dir[2]		= 0.0f;
				}
				else
				{
					Particles[a].Effect->RenderParticle(&Particles[a]);
				}
			}
		}
		PrepairRender();
	}
	Render();
}

void EffectsSystem::Render()
{
}

void	EffectsSystem::Remove	(BaseEffect* Effect								)
{
	Effect->KillMe();
}

void	EffectsSystem::DrawScreenFacingQuad(BlendMode bm, int TextureID , float PosScale[5])
{
	NumberOfQuads++;
	int WhereItShouldGo = TextureID + ( ( (int) bm ) * NumberOfTextures );
	for (int a = (NumberOfTextures*NumberOfBlendModes)-1; a > WhereItShouldGo; a--)
	{
		int start = RenderInformation.TextureIDs[a]-RenderInformation.TextureIDs[a-1];
		int end	  = RenderInformation.TextureIDs[a];
		for (int b=0; b<4; b++)
		{
			Verts[(end*4)+b].normal		= Verts[(start*4)+b].normal;
			Verts[(end*4)+b].position	= Verts[(start*4)+b].position;
			Verts[(end*4)+b].tangent	= Verts[(start*4)+b].tangent;
			Verts[(end*4)+b].UVs		= Verts[(start*4)+b].UVs;
		}
	}
	DrawScreenFacingQuad((RenderInformation.TextureIDs[WhereItShouldGo]*4),PosScale);	
	for (int a=WhereItShouldGo; a<NumberOfBlendModes*NumberOfTextures; a++)
	{
		RenderInformation.TextureIDs[a]++;
	}
}

void	EffectsSystem::DrawScreenFacingQuad(BlendMode bm, int TextureID , D3DXVECTOR3 Pos,D3DXVECTOR2 Scale,D3DXVECTOR3 Colour)
{
	NumberOfQuads++;
	int WhereItShouldGo = TextureID + ( ( (int) bm ) * NumberOfTextures );
	for (int a = (NumberOfTextures*NumberOfBlendModes)-1; a > WhereItShouldGo; a--)
	{
		int start = RenderInformation.TextureIDs[a]-RenderInformation.TextureIDs[a-1];
		int end	  = RenderInformation.TextureIDs[a];
		for (int b=0; b<4; b++)
		{
			Verts[(end*4)+b].normal		= Verts[(start*4)+b].normal;
			Verts[(end*4)+b].position	= Verts[(start*4)+b].position;
			Verts[(end*4)+b].tangent	= Verts[(start*4)+b].tangent;
			Verts[(end*4)+b].UVs		= Verts[(start*4)+b].UVs;
		}
	}
	float p[5]={Pos.x,Pos.y,Pos.z,Scale.x,Scale.y};
	int firstvert = (RenderInformation.TextureIDs[WhereItShouldGo]*4);
	Verts[firstvert].tangent = Colour;
	Verts[firstvert+1].tangent = Colour;
	Verts[firstvert+2].tangent = Colour;
	Verts[firstvert+3].tangent = Colour;

	DrawScreenFacingQuad((RenderInformation.TextureIDs[WhereItShouldGo]*4),p);	
	for (int a=WhereItShouldGo; a<NumberOfBlendModes*NumberOfTextures; a++)
	{
		RenderInformation.TextureIDs[a]++;
	}
}

void	EffectsSystem::DrawScreenFacingQuad(BlendMode bm, int TextureID , D3DXVECTOR3 Pos,D3DXVECTOR2 Scale)
{
	NumberOfQuads++;
	int WhereItShouldGo = TextureID + ( ( (int) bm ) * NumberOfTextures );
	for (int a = (NumberOfTextures*NumberOfBlendModes)-1; a > WhereItShouldGo; a--)
	{
		int start = RenderInformation.TextureIDs[a]-RenderInformation.TextureIDs[a-1];
		int end	  = RenderInformation.TextureIDs[a];
		for (int b=0; b<4; b++)
		{
			Verts[(end*4)+b].normal		= Verts[(start*4)+b].normal;
			Verts[(end*4)+b].position	= Verts[(start*4)+b].position;
			Verts[(end*4)+b].tangent	= Verts[(start*4)+b].tangent;
			Verts[(end*4)+b].UVs		= Verts[(start*4)+b].UVs;
		}
	}
	float p[5]={Pos.x,Pos.y,Pos.z,Scale.x,Scale.y};
	DrawScreenFacingQuad((RenderInformation.TextureIDs[WhereItShouldGo]*4),p);	
	for (int a=WhereItShouldGo; a<NumberOfBlendModes*NumberOfTextures; a++)
	{
		RenderInformation.TextureIDs[a]++;
	}
}

void EffectsSystem::ResetCounters()
{
	NumberOfQuads = 0;
	for (int a = 0; a < (NumberOfTextures*NumberOfBlendModes); a++)
	{
		RenderInformation.TextureIDs[a]=0;
	}
}

void	EffectsSystem::PrepairRender()
{
	if (NumberOfQuads)
	{
		Vertex *ptr = NULL;
		(*VertexBuffer).Lock(0, 0, (void**)&ptr, 0);
		memcpy((void*)ptr, (void*)Verts, NumberOfQuads*4*sizeof(Vertex));
		(*VertexBuffer).Unlock();
	}
}


bool	EffectsSystem::CreateVertexBuffer()
{
	HRESULT hr;
	hr = VisualInterface::I()->Naughty()->CreateVertexBuffer(NumberOfVertices*sizeof(Vertex),0,0,D3DPOOL_MANAGED,&VertexBuffer,0);
	hr = VisualInterface::I()->Naughty()->CreateIndexBuffer((NumberOfVertices/2)*3*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&IndexBuffer,0);
	WORD	*CubeIndex	= new WORD[(NumberOfVertices/2)*3];
	
	for (int a=0; a<NumberOfVertices; a+=4)
	{
		Verts[a].position.x= 1.0f;	Verts[a].normal.x=0.0f;	Verts[a].UVs.x=1.0f;
		Verts[a].position.y= 1.0f;	Verts[a].normal.y=0.0f;	Verts[a].UVs.y=1.0f;
		Verts[a].position.z= 0.0f;	Verts[a].normal.z=1.0f;

		Verts[a+1].position.x= 1.0f;	Verts[a+1].normal.x=0.0f;	Verts[a+1].UVs.x=1.0f;
		Verts[a+1].position.y=-1.0f;	Verts[a+1].normal.y=0.0f;	Verts[a+1].UVs.y=0.0f;
		Verts[a+1].position.z= 0.0f;	Verts[a+1].normal.z=1.0f;

		Verts[a+2].position.x=-1.0f;	Verts[a+2].normal.x=0.0f;	Verts[a+2].UVs.x=0.0f;
		Verts[a+2].position.y=-1.0f;	Verts[a+2].normal.y=0.0f;	Verts[a+2].UVs.y=0.0f;
		Verts[a+2].position.z= 0.0f;	Verts[a+2].normal.z=1.0f;

		Verts[a+3].position.x=-1.0f;	Verts[a+3].normal.x=0.0f;	Verts[a+3].UVs.x=0.0f;
		Verts[a+3].position.y= 1.0f;	Verts[a+3].normal.y=0.0f;	Verts[a+3].UVs.y=1.0f;
		Verts[a+3].position.z= 0.0f;	Verts[a+3].normal.z=1.0f;
	}
	for (int a=0; a<(NumberOfVertices/2)*3; a+=6)
	{
		CubeIndex[a+0]=a+0;
		CubeIndex[a+1]=a+1;
		CubeIndex[a+2]=a+2;
		CubeIndex[a+3]=a+0;
		CubeIndex[a+4]=a+2;
		CubeIndex[a+5]=a+3;
	}

	Vertex *ptr = NULL;
	(*VertexBuffer).Lock(0, 0, (void**)&ptr, 0);
	memcpy((void*)ptr, (void*)Verts, NumberOfVertices*sizeof(Vertex));
	(*VertexBuffer).Unlock();

	WORD *iptr = NULL;

	(*IndexBuffer).Lock(0, 0, (void**)&iptr, 0);
	memcpy((void*)iptr, (void*)CubeIndex, (NumberOfVertices/2)*3*sizeof(WORD));
	(*IndexBuffer).Unlock();
	delete [] CubeIndex;

	return true;
}

void	EffectsSystem::DrawScreenFacingQuad(int firstvert,float PosScale[5])
{
	D3DXMATRIX	MatrixView , MatrixBill;
	Camera *c;
	VisualInterface::I()->GetCamera(&c);
	D3DXMatrixLookAtLH( &MatrixView, &c->Position, &c->Interest, &c->Up );
	MatrixView._41 = 0.0f;
	MatrixView._42 = 0.0f;
	MatrixView._43 = 0.0f;
	D3DXMatrixInverse( &MatrixBill, NULL, &MatrixView );
	MatrixBill._41 = PosScale[0];
	MatrixBill._42 = PosScale[1];
	MatrixBill._43 = PosScale[2];
	D3DXVECTOR4 v4;
	D3DXVECTOR3 v3(PosScale[3],PosScale[4],0.0f);
	D3DXVec3Transform(&v4,&v3,&MatrixBill);
	Verts[firstvert+0].position.x = v4.x;
	Verts[firstvert+0].position.y = v4.y;
	Verts[firstvert+0].position.z = v4.z;
	Verts[firstvert+0].UVs.x	  = 1.0f;
	Verts[firstvert+0].UVs.y	  = 1.0f;

	v3=D3DXVECTOR3(PosScale[3],-PosScale[4],0.0f);
	D3DXVec3Transform(&v4,&v3,&MatrixBill);
	Verts[firstvert+1].position.x = v4.x;
	Verts[firstvert+1].position.y = v4.y;
	Verts[firstvert+1].position.z = v4.z;
	Verts[firstvert+1].UVs.x	  = 1.0f;
	Verts[firstvert+1].UVs.y	  = 0.0f;

	v3=D3DXVECTOR3(-PosScale[3],-PosScale[4],0.0f);
	D3DXVec3Transform(&v4,&v3,&MatrixBill);
	Verts[firstvert+2].position.x = v4.x;
	Verts[firstvert+2].position.y = v4.y;
	Verts[firstvert+2].position.z = v4.z;
	Verts[firstvert+2].UVs.x	  = 0.0f;
	Verts[firstvert+2].UVs.y	  = 0.0f;

	v3=D3DXVECTOR3(-PosScale[3],PosScale[4],0.0f);
	D3DXVec3Transform(&v4,&v3,&MatrixBill);
	Verts[firstvert+3].position.x = v4.x;
	Verts[firstvert+3].position.y = v4.y;
	Verts[firstvert+3].position.z = v4.z;
	Verts[firstvert+3].UVs.x	  = 0.0f;
	Verts[firstvert+3].UVs.y	  = 1.0f;
	//////////////////////////////////////////////////////////////////////////
}
