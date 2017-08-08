#include "VisualInterface.h"
#include "obj\OBJIO.h"
#include "..\xml\tsf\include\tsf\tsf.hpp"

VisualInterface *	VisualInterface::Instance = NULL;
VisualInterface *	VisualInterface::I()
{
	if ( !Instance )
	{
		Instance = new VisualInterface();
	}
	return Instance;
}

bool VisualInterface::Initialise(const bool fscreen)
{
	isfullscreen=fscreen;
	if (!isinitialised)
	{
		isinitialised = InitialiseDirect3D();
	}
	if (isinitialised)
	{
		InitialiseVertexDeclaration();
		LoadEffect("shaders\\Default9.fx");
	}
	return isinitialised;
}

VisualInterface::~VisualInterface()
{
	if (isinitialised)
	{
		CloseDownDirect3D();
	}
}

void VisualInterface::CloseDownDirect3D()
{
	if (gD3dDevice)
	{
		gD3dDevice->Release();
		gD3dDevice=NULL;
	}
	if (gD3dObject)
	{
		gD3dObject->Release();
		gD3dObject=NULL;
	}
}

bool VisualInterface::Update()
{
	HRESULT hr = gD3dDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(r,g,b),1.0f,0);	//clears the zbuffer and the screen to the rgb values

	if (SUCCEEDED(gD3dDevice->BeginScene()))	//gets the D3D device ready to draw the screen
	{
		RenderModels();
		hr = gD3dDevice->EndScene();
		hr = gD3dDevice->Present(0,0,0,0);		//places final render to the screen
		return true;
	}
	return false;
}


int VisualInterface::LoadEffect				(	const char* filename		)
{
	LPD3DXEFFECT newEffect;
	LPD3DXBUFFER	code = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(gD3dDevice,filename,NULL,NULL,0,NULL,&newEffect,&code);
	if (hr==E_FAIL)
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	else
	{
		effects.push_back(newEffect);
		return (((int)effects.size())-1);
	}
	return -1;
}

int VisualInterface::loadmesh(const char* FileName)
{
	OBJIO t;
	Mesh * test=new Mesh;
//	t.load(FileName,test);
	//just for now
	test->triangle();
	Meshes.push_back(test);
	Model* m=new Model;
	m->EffectID=0;
	m->MeshID=0;
	Models.push_back(m);
	return (int)Meshes.size()-1;
}

void VisualInterface::LoadScene					( const char* FileName			)
{
	tsf::scene_ptr scene;
	scene = tsf::read_scene_from_file( FileName );
	if ( scene == 0 )
	{
		return;
	}
	int numberofmodels = (int)scene->models.size();
	for (int a = 0; a < numberofmodels; a++)
	{
//		scene->models[a]->
	}
}

