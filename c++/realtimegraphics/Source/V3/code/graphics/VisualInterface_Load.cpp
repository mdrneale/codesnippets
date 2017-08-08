#include "VisualInterface.h"
#include "shading/DrawMethod_simple.h"
#include "shading/DrawMethod_ChinesePainting.h"
#include "shading/DrawMethod_deferred.h"
#include "obj\OBJIO.h"
#include "..\xml\tsf\include\tsf\tsf.hpp"
#include <hash_map>
#include "vertex/Vertex.h"
using namespace std;

VisualInterface::VisualInterface() :	
isinitialised				( false	),
isfullscreen				( false	),
gD3dObject					( NULL	),
gD3dDevice					( NULL  ),
r							( 50	),
g							( 100	),
b							( 200	),
ScreenWidth					( WIDTH	),
ScreenHeight				( HEIGHT)
{
}

VisualInterface *	VisualInterface::Instance = NULL;

VisualInterface *	VisualInterface::I()
{
	if ( !Instance )
	{
		Instance = new VisualInterface();
	}
	return Instance;
}

bool VisualInterface::InitialiseDirect3D()
{
	HWND hWnd	= FindWindow("MyWindowClass",NULL);
	gD3dObject	= Direct3DCreate9(D3D_SDK_VERSION);	
	if (gD3dObject==NULL)
	{
		return false;
	}
	D3DPRESENT_PARAMETERS presParams;
	ZeroMemory(&presParams,sizeof(presParams));

	if (isfullscreen)
	{
		presParams.Windowed						=	FALSE;
		presParams.SwapEffect					=	D3DSWAPEFFECT_DISCARD;
		presParams.BackBufferCount				=	1;
		presParams.BackBufferWidth				=	ScreenWidth;
		presParams.BackBufferHeight				=	ScreenHeight;
		presParams.BackBufferFormat				=	((_D3DFORMAT)22);
		presParams.PresentationInterval			=	D3DPRESENT_INTERVAL_IMMEDIATE;
		presParams.EnableAutoDepthStencil		=	TRUE;
		presParams.AutoDepthStencilFormat		=	D3DFMT_D24X8;
		presParams.hDeviceWindow				=	hWnd;
//		presParams.BackBufferWidth				=	ScreenWidth;
//		presParams.BackBufferHeight				=	ScreenHeight;
//		presParams.BackBufferFormat				=	D3DFMT_X8R8G8B8;
//		presParams.PresentationInterval			=	D3DPRESENT_INTERVAL_IMMEDIATE;
//		presParams.MultiSampleType				=	D3DMULTISAMPLE_NONE;
		presParams.FullScreen_RefreshRateInHz	=	D3DPRESENT_RATE_DEFAULT;
		presParams.Flags						=	D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
//		presParams.EnableAutoDepthStencil		=	TRUE;
//		presParams.AutoDepthStencilFormat		=	D3DFMT_D24X8;
//		presParams.hDeviceWindow				=	hWnd;
	}
	else
	{
		presParams.Windowed						=	TRUE;
		presParams.SwapEffect					=	D3DSWAPEFFECT_DISCARD;
		presParams.BackBufferCount				=	1;
		presParams.BackBufferWidth				=	ScreenWidth;
		presParams.BackBufferHeight				=	ScreenHeight;
		presParams.BackBufferFormat				=	((_D3DFORMAT)22);
		presParams.PresentationInterval			=	D3DPRESENT_INTERVAL_IMMEDIATE;
		presParams.EnableAutoDepthStencil		=	TRUE;
		presParams.AutoDepthStencilFormat		=	D3DFMT_D24X8;
		presParams.hDeviceWindow				=	hWnd;
	}
	// Set default settings
UINT AdapterToUse=D3DADAPTER_DEFAULT;
D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL;
#if SHIPPING_VERSION
// When building a shipping version, disable PerfHUD (opt-out)
#else
// Look for 'NVIDIA PerfHUD' adapter
// If it is present, override default settings
for (UINT Adapter=0;Adapter<gD3dObject->GetAdapterCount();Adapter++)
{
D3DADAPTER_IDENTIFIER9 Identifier;
HRESULT Res;
Res = gD3dObject->GetAdapterIdentifier(Adapter,0,&Identifier);
if (strstr(Identifier.Description,"PerfHUD") != 0)
{
AdapterToUse=Adapter;
DeviceType=D3DDEVTYPE_REF;
break;
}
}
#endif
	// Get the Direct3D object to give us a pointer to a device interface object
	if (FAILED(gD3dObject->CreateDevice(AdapterToUse,DeviceType,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,	&presParams, &gD3dDevice)))
	{	// if the hardware is not good enough use a mixture of software and hardware
		if (FAILED(gD3dObject->CreateDevice(AdapterToUse,DeviceType,hWnd,D3DCREATE_MIXED_VERTEXPROCESSING, &presParams, &gD3dDevice)))
		{	// if the hardware cant handle a mix use only software
			if (FAILED(gD3dObject->CreateDevice(AdapterToUse,DeviceType,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,	&presParams, &gD3dDevice)))
			{	// if that isn't good enough buy a new PC.
				return false;
			}
		}
	}
	float	FieldOfView	= D3DX_PI / 4.0f;
	float	NearPlane	= 1.0f;
	float	FarPlane	= 1000.0f;
	SetProjection(FieldOfView,NearPlane,FarPlane);
	gD3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	gD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return true;
}

void VisualInterface::SetVertexDeclaration	(											)
{
gD3dDevice->SetVertexDeclaration(vd);
}

void VisualInterface::InitialiseVertexDeclaration	(								)
{
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL  , 0},
		{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
		{0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	VertexSize=44;
	gD3dDevice->CreateVertexDeclaration(decl,&vd);
	SetVertexDeclaration();
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
		HRESULT hr	= gD3dDevice->GetRenderTarget(0,&drawMethodOutput.Render);
		hr			= gD3dDevice->GetDepthStencilSurface(&drawMethodOutput.Depth);
		scene.AmbientColour.B=0.15f;
		scene.AmbientColour.G=0.15f;
		scene.AmbientColour.R=0.15f;
		LoadEffect("shaders\\chrome.fx");
		DrawMethod * dm = new DrawMethod_ChinesePainting(gD3dDevice,&scene);
		drawMethods.push_back(dm);

		scene.CurrentCamera=2;
		Camera *c1= new Camera();
		scene.Cameras.push_back(c1);
		Camera *c2= new Camera();
		scene.Cameras.push_back(c2);

		makeit();

//		ht.Init();
	}
	return isinitialised;
}

void VisualInterface::makeit()
{
	OBJIO loader;
	Mesh* a = new Mesh;
	loader.load("models\\pyramid.obj",a,gD3dDevice);
	scene.Meshes.push_back(a);
	LightBox = scene.Meshes.size()-1;
//
//	Model *Room			= new Model;
//	Room->MeshID		= RoomMeshID;
//	Room->EffectID		= 0;
	LoadTexture("textures\\box.tga");
	LoadTexture("textures\\bricknormal.tga");
////	Room->Scaling	*=10.0f;
//	Room->Position.y=-5.0f;
//	scene.Models.push_back(Room);
	
	LoadScene("models\\sponza\\sponza.tsf");
}

int VisualInterface::LoadEffect				(	const char* filename		)
{
	EffectData*		NED=new EffectData();
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(gD3dDevice,filename,NULL,NULL,0,NULL,&NED->Effect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
		delete NED;
	}
	else if (NED->Effect)
	{
		NED->NeedsColMap=true;
		NED->NeedsPosMap=true;
		NED->NeedsNormMap=true;
		scene.effects.push_back(NED);
		return (((int)scene.effects.size())-1);
	}
	else
	{
		delete NED;
	}
	return -1;
}

int VisualInterface::LoadTexture			( const char* FileName			)
{
	LPDIRECT3DTEXTURE9 newTexture=NULL;
	D3DXCreateTextureFromFile(gD3dDevice,FileName,&newTexture);
	if (newTexture)
	{
		scene.Textures.push_back(newTexture);
	}
	return (int)scene.Textures.size()-1;
}

void VisualInterface::LoadScene					( const char* FileName			)
{
	static int n = LoadTexture("textures\\norm.tga");
	static int a=LoadTexture("textures\\vase_ddn.tga");
	static int b=LoadTexture("textures\\sponza_thorn_ddn.tga");
	tsf::scene_ptr scene;
	scene = tsf::read_scene_from_file( FileName,true );
	if ( scene == 0 )
	{
		return;
	}
	tsf::model_map::iterator ModelIter;
	for ( ModelIter = scene->models.begin(); ModelIter != scene->models.end(); ModelIter++ )
	{
		Model	*NewModel					= new Model();
		Mesh	*NewModel_Mesh				= new Mesh();

		std::string ModelName = ModelIter->second->geometry;
		std::string ModelName2= ModelIter->first;
		NewModel->normalMapID=n;
		
		if (ModelName2 == "atrium_floor" || ModelName2 == "atrium")
		{
			static int a=LoadTexture("textures\\spnza_bricks_a_ddn.tga");
			NewModel->normalMapID=a;
		}
		else if (ModelName2 == "lion_mount03" || ModelName2 == "lion_mount02" || ModelName2 == "lion_mount01")
		{
			static int a=LoadTexture("textures\\background_ddn.tga");
			NewModel->normalMapID=a;
		}
		else if (ModelName2 == "lion_head01" || ModelName2 == "lion_head02" || ModelName2 == "lion_head03")
		{
			static int a=LoadTexture("textures\\lion_ddn.tga");
			NewModel->normalMapID=a;
		}
		else if (ModelName2 == "vase01" || ModelName2 == "vase02" || ModelName2 == "vase03" || ModelName2 == "vase04" || ModelName2 == "vase05" || ModelName2 == "vase06")
		{
			NewModel->normalMapID=a;
		}

		tsf::model	current_model			= *(ModelIter->second);

		tsf::material_ptr map = scene->materials[current_model.materials["base"]];
		tsf::symbol_map::iterator	m		= current_model.materials.begin();
		std::string materialtype = (*m).second;
		if (materialtype == "metal_mat" || materialtype == "vase_mat")
		{
			NewModel->EffectID=0;
			NewModel->normalMapID=a;
		}
		if (materialtype == "leaf_mat")
		{
			NewModel->normalMapID=b;
		}

		const tsf::parameter_map pmp = map->parameters;
		if (map->type=="lambert")
		{
			const tsf::colour *col		= tsf::parameter_get<tsf::colour>( "colour", pmp );
			NewModel->BaseCol[0] = col->r;
			NewModel->BaseCol[1] = col->g;
			NewModel->BaseCol[2] = col->b;
		}
		else if (map->type=="phong")
		{
			const tsf::colour *col		= tsf::parameter_get<tsf::colour>( "colour", pmp );
			const tsf::colour *speccol	= tsf::parameter_get<tsf::colour>( "specularColour", pmp );
			const float	*power			= tsf::parameter_get<float>( "cosPower", pmp );
			NewModel->BaseCol[0] = col->r;
			NewModel->BaseCol[1] = col->g;
			NewModel->BaseCol[2] = col->b;
			NewModel->SpecCol[0] = speccol->r;
			NewModel->SpecCol[1] = speccol->g;
			NewModel->SpecCol[2] = speccol->b;
			NewModel->SpecPow	 = *power;
		}
		


		tsf::node	rootnode				= *(scene->graphs[scene->root_graph]);
		for ( tsf::node_list::iterator ni	= rootnode.children.begin(); ni != rootnode.children.end(); ni++ )
		{
			if ((*ni)->name == ModelName2)
			{
				NewModel->Position.x=(*ni)->translate.x;
				NewModel->Position.y=(*ni)->translate.y;
				NewModel->Position.z=(*ni)->translate.z;
				NewModel->Rotation.x=(*ni)->rotate.x*D3DX_PI*0.25f;
				NewModel->Rotation.y=(*ni)->rotate.y*D3DX_PI*0.25f;
				NewModel->Rotation.z=(*ni)->rotate.z*D3DX_PI*0.25f;
				NewModel->Scaling.x=(*ni)->scale.x;
				NewModel->Scaling.y=(*ni)->scale.y;
				NewModel->Scaling.z=(*ni)->scale.z;
				break;
			}
		}
		std::string current_models_MeshName	= current_model.geometry;
		tsf::mesh	current_models_Mesh		= *(scene->meshes[current_models_MeshName]);
		tsf::polygon_batch::iterator pbi	= current_models_Mesh.polygons.begin()->second->begin();
		unsigned int EstimatedTotalIndices	= 6*current_models_Mesh.polygons.begin()->second->size();
		unsigned int *Index					= new unsigned int[EstimatedTotalIndices];
		unsigned int TotalIndices			= 0;
		for ( tsf::polygon_batch::iterator pbi	= current_models_Mesh.polygons.begin()->second->begin(); pbi != current_models_Mesh.polygons.begin()->second->end(); pbi++ )
		{
			unsigned int points = pbi->size();
			if (points==4)
			{
				Index[TotalIndices  ] = (*pbi)[0];
				Index[TotalIndices+1] = (*pbi)[1];
				Index[TotalIndices+2] = (*pbi)[2];
				Index[TotalIndices+3] = (*pbi)[2];
				Index[TotalIndices+4] = (*pbi)[3];
				Index[TotalIndices+5] = (*pbi)[0];
				TotalIndices+=6;
			}
			else if (points==3)
			{
				Index[TotalIndices  ] = (*pbi)[0];
				Index[TotalIndices+1] = (*pbi)[1];
				Index[TotalIndices+2] = (*pbi)[2];
				TotalIndices+=3;			
			}
			else
			{
				break;
			}
		}
		NewModel_Mesh->PrimitiveCount=TotalIndices/3;
		HRESULT hr = gD3dDevice->CreateIndexBuffer( TotalIndices*sizeof(int),D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_MANAGED,&NewModel_Mesh->_ib,0);
		unsigned int *iptr = NULL;
		(*NewModel_Mesh->_ib).Lock(0, 0, (void**)&iptr, 0);
		memcpy((void*)iptr, (void*)Index, TotalIndices*sizeof(int));
		(*NewModel_Mesh->_ib).Unlock();

		tsf::point_array		positions	= current_models_Mesh.position_array;
		tsf::vector_array		normals		= current_models_Mesh.normal_array;
		tsf::vector_array		tangents	= current_models_Mesh.tangent_array;
		tsf::coordinate_array   texcoords	= current_models_Mesh.texcoord_array;
		unsigned int TotalVertices			= positions.size();
		NewModel_Mesh->NumofVerts			= TotalVertices;
		Vertex *Points	= new Vertex[TotalVertices];
		for (unsigned int a=0; a<TotalVertices; a++)
		{
			Points[a].normal.x=normals[a].x;
			Points[a].normal.y=normals[a].y;
			Points[a].normal.z=normals[a].z;

			Points[a].position.x=positions[a].x;
			Points[a].position.y=positions[a].y;
			Points[a].position.z=positions[a].z;

			Points[a].tangent.x=tangents[a].x;
			Points[a].tangent.y=tangents[a].y;
			Points[a].tangent.z=tangents[a].z;

			Points[a].UVs.x=texcoords[a].s;
			Points[a].UVs.y=texcoords[a].t;
		}
		hr = gD3dDevice->CreateVertexBuffer(NewModel_Mesh->NumofVerts*sizeof(Vertex),0,0,D3DPOOL_MANAGED,&NewModel_Mesh->_vb,0);
		Vertex *ptr = NULL;

		(*NewModel_Mesh->_vb).Lock(0, 0, (void**)&ptr, 0);
		memcpy((void*)ptr, (void*)Points, NewModel_Mesh->NumofVerts*sizeof(Vertex));
		(*NewModel_Mesh->_vb).Unlock();
		delete [] Points;
		delete [] Index;

		VisualInterface::scene.Meshes.push_back(NewModel_Mesh);
		NewModel->MeshID= VisualInterface::scene.Meshes.size() -1;
		VisualInterface::scene.Models.push_back(NewModel);
	}
}

int VisualInterface::loadmesh(const char* FileName)
{
//	OBJIO t;
//	Mesh * test=new Mesh;
//	t.load(FileName,test);
	//just for now
//	test->triangle();
//	Meshes.push_back(test);
//	Model* m=new Model;
//	m->EffectID=0;
//	m->MeshID=0;
//	Models.push_back(m);
//	return (int)Meshes.size()-1;
	return -1;
}

int VisualInterface::addmodel				( Model& ModelToAdd)
{
	Model *newModel					= new Model;
	newModel->diffuseMapID			= ModelToAdd.diffuseMapID;
	newModel->EffectID				= ModelToAdd.EffectID;
	newModel->MaterialID			= ModelToAdd.MaterialID;
	newModel->MeshID				= ModelToAdd.MeshID;
	newModel->Position				= ModelToAdd.Position;
	newModel->Rotation				= ModelToAdd.Rotation;
	newModel->Scaling				= ModelToAdd.Scaling;
	newModel->SpecularHeightMapID	= ModelToAdd.SpecularHeightMapID;
	scene.Models.push_back(newModel);
	return (int(scene.Models.size()-1));
}

int VisualInterface::AddCamera				( Camera& CamToAdd									)
{
	Camera * newcam		= new Camera;
	newcam->Position	= CamToAdd.Position;
	newcam->Interest	= CamToAdd.Interest;
	newcam->Up			= CamToAdd.Up;
	newcam->FOV			= CamToAdd.FOV;
	scene.Cameras.push_back(newcam);
	return (int(scene.Cameras.size()-1));
}

int VisualInterface::AddLight				( Light& LightToAdd									)
{
	Light * newlight	= new Light;
	newlight->Position	= LightToAdd.Position;
	newlight->Interest	= LightToAdd.Interest;
	newlight->Up		= LightToAdd.Up;
	newlight->FOV		= LightToAdd.FOV;
	newlight->InnerFOV	= LightToAdd.InnerFOV;
	newlight->Attenuation=LightToAdd.Attenuation;
	newlight->LightBox	= LightToAdd.LightBox;
	newlight->Col.B		= LightToAdd.Col.B;
	newlight->Col.G		= LightToAdd.Col.G;
	newlight->Col.R		= LightToAdd.Col.R;
	scene.Lights.push_back(newlight);
	return (int(scene.Lights.size()-1));
}

void VisualInterface::GetLight				( int lightID , Light** light							)
{
	*light = scene.Lights[lightID];
}

void VisualInterface::GetCamera				( Camera** Cam							)
{
	*Cam = scene.Cameras[scene.CurrentCamera];
}

void VisualInterface::NextCamera()
{
	if ( (++scene.CurrentCamera) >= scene.Cameras.size() )
	{
		scene.CurrentCamera = 0;
	}
}
