#include "Mesh.h"
#include "..\VisualInterface.h"
#include "..\vertex\Vertex.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if (_vb)
	{
		_vb->Release();
	}
	if (_ib)
	{
		_ib->Release();
	}
}
void Mesh::triangle(LPDIRECT3DDEVICE9		gD3dDevice)
{
	NumofVerts		=4;
	PrimitiveCount	=2;
	HRESULT hr;
	hr = gD3dDevice->CreateVertexBuffer(NumofVerts*sizeof(Vertex),0,0,D3DPOOL_MANAGED,&_vb,0);
	hr = gD3dDevice->CreateIndexBuffer( PrimitiveCount*3*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&_ib,0);
	Vertex *CubePoints	= new Vertex[NumofVerts];
	WORD	*CubeIndex	= new WORD[PrimitiveCount*3];

	CubePoints[0].position.x= 1.0f;	CubePoints[0].normal.x=0.0f;	CubePoints[0].UVs.x=1.0f;
	CubePoints[0].position.y= 1.0f;	CubePoints[0].normal.y=0.0f;	CubePoints[0].UVs.y=1.0f;
	CubePoints[0].position.z= 0.0f;	CubePoints[0].normal.z=1.0f;

	CubePoints[1].position.x= 1.0f;	CubePoints[1].normal.x=0.0f;	CubePoints[1].UVs.x=1.0f;
	CubePoints[1].position.y=-1.0f;	CubePoints[1].normal.y=0.0f;	CubePoints[1].UVs.y=0.0f;
	CubePoints[1].position.z= 0.0f;	CubePoints[1].normal.z=1.0f;

	CubePoints[2].position.x=-1.0f;	CubePoints[2].normal.x=0.0f;	CubePoints[2].UVs.x=0.0f;
	CubePoints[2].position.y=-1.0f;	CubePoints[2].normal.y=0.0f;	CubePoints[2].UVs.y=0.0f;
	CubePoints[2].position.z= 0.0f;	CubePoints[2].normal.z=1.0f;

	CubePoints[3].position.x=-1.0f;	CubePoints[3].normal.x=0.0f;	CubePoints[3].UVs.x=0.0f;
	CubePoints[3].position.y= 1.0f;	CubePoints[3].normal.y=0.0f;	CubePoints[3].UVs.y=1.0f;
	CubePoints[3].position.z= 0.0f;	CubePoints[3].normal.z=1.0f;

	CubeIndex[0]=0;
	CubeIndex[1]=1;
	CubeIndex[2]=2;
	CubeIndex[3]=0;
	CubeIndex[4]=2;
	CubeIndex[5]=3;

	Vertex *ptr = NULL;

	(*_vb).Lock(0, 0, (void**)&ptr, 0);
	memcpy((void*)ptr, (void*)CubePoints, NumofVerts*sizeof(Vertex));
	(*_vb).Unlock();

	WORD *iptr = NULL;

	(*_ib).Lock(0, 0, (void**)&iptr, 0);
	memcpy((void*)iptr, (void*)CubeIndex, PrimitiveCount*3*sizeof(WORD));
	(*_ib).Unlock();

	delete [] CubePoints;
	delete [] CubeIndex;
}

//void Mesh::Initialise(tsf::mesh &TyronesMesh)
//{
	//NumofVerts		=(int)TyronesMesh.position_array.size();
	//PrimitiveCount	=(int)TyronesMesh.polygons.size()/3;
	//HRESULT hr;
	//hr = VisualInterface::I()->gD3dDevice->CreateVertexBuffer(NumofVerts*sizeof(Vertex),0,0,D3DPOOL_MANAGED,&_vb,0);
	//hr = VisualInterface::I()->gD3dDevice->CreateIndexBuffer( PrimitiveCount*3*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&_ib,0);

	//Vertex *CubePoints	= new Vertex[NumofVerts];
	//WORD	*CubeIndex	= new WORD[PrimitiveCount*3];

	//for (int a=0; a < (int)TyronesMesh.position_array.size(); a++)
	//{
	//	CubePoints[a].position.x=TyronesMesh.position_array[a].x;
	//	CubePoints[a].position.y=TyronesMesh.position_array[a].y;
	//	CubePoints[a].position.z=TyronesMesh.position_array[a].z;
	//	CubePoints[a].normal.x=TyronesMesh.normal_array[a].x;
	//	CubePoints[a].normal.y=TyronesMesh.normal_array[a].y;
	//	CubePoints[a].normal.z=TyronesMesh.normal_array[a].z;
	//	CubePoints[a].tangent.x=TyronesMesh.tangent_array[a].x;
	//	CubePoints[a].tangent.y=TyronesMesh.tangent_array[a].y;
	//	CubePoints[a].tangent.z=TyronesMesh.tangent_array[a].z;
	//	CubePoints[a].UVs.x=TyronesMesh.texcoord_array[a].s;
	//	CubePoints[a].UVs.y=TyronesMesh.texcoord_array[a].t;
	//}
	////polygon_batch_ptr test = TyronesMesh.polygons.begin();
	////int b=0;
	////for (int a=0; a < (int)TyronesMesh.polygons.size(); a++)
	////{
	////	CubeIndex[b]	=(WORD)test[0].get().iterator->at(0);
	////	CubeIndex[b+1]	=(WORD)test[0].get().iterator->at(1);
	////	CubeIndex[b+2]	=(WORD)test[0].get().iterator->at(2);
	////	b+=3;
	////}

	//Vertex *ptr = NULL;

	//(*_vb).Lock(0, 0, (void**)&ptr, 0);
	//memcpy((void*)ptr, (void*)CubePoints, NumofVerts*sizeof(Vertex));
	//(*_vb).Unlock();

	//WORD *iptr = NULL;

	//(*_ib).Lock(0, 0, (void**)&iptr, 0);
	//memcpy((void*)iptr, (void*)CubeIndex, PrimitiveCount*3*sizeof(WORD));
	//(*_ib).Unlock();

//}
