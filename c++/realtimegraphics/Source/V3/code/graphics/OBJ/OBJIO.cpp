#include "OBJIO.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <d3d9.h>
#include "..\vertex\Vertex.h"

struct indexstruct 
{
	unsigned int vec;
	unsigned int nor;
	unsigned int tex;
};
void OBJIO::load(const char* file,Mesh* meshtofill,LPDIRECT3DDEVICE9		gD3dDevice)
{
	std::ifstream infile;
	infile.open(file);
	std::vector<D3DXVECTOR3> pos;
	std::vector<D3DXVECTOR3> ind;
	std::vector<D3DXVECTOR3> nor;
	std::vector<D3DXVECTOR2> tex;
	std::vector<indexstruct> inds;


	if (infile)
	{
		meshtofill->NumofVerts=0;
		meshtofill->PrimitiveCount=0;
		char objLine[256];
		while( !infile.eof() )
		{
			infile.getline( objLine, 255 );
			std::stringstream ss;
			char str;
			// position	////////////////////////////////////////////////////////////////////////
			if( objLine[0] == 'v' && objLine[1] == ' ')
			{
				ss << objLine;
				D3DXVECTOR3 ppos;
				ss >> str >> ppos.x >> ppos.y >> ppos.z;
				pos.push_back(ppos);
			}
			// textureCoordinates //////////////////////////////////////////////////////////////
			else if( objLine[0] == 'v' && objLine[1] == 't')
			{
				ss << objLine;
				D3DXVECTOR2 ptex;
				ss >> str >> str >> ptex.x >> ptex.y;
				tex.push_back(ptex);
			}
			// normal //////////////////////////////////////////////////////////////////////////
			else if( objLine[0] == 'v' && objLine[1] == 'n')
			{
				ss << objLine;
				D3DXVECTOR3 pnor;
				ss >> str >> str >> pnor.x >> pnor.y >> pnor.z;
				nor.push_back(pnor);
			}
			// rearrange ///////////////////////////////////////////////////////////////////////
			else if( objLine[0] == 'f')
			{
				meshtofill->NumofVerts+=3;
				meshtofill->PrimitiveCount++;
				ss << objLine;
				indexstruct t1;
				indexstruct t2;
				indexstruct t3;
				ss >> str	>> t1.vec >> str >> t1.tex >> str >> t1.nor
							>> t2.vec >> str >> t2.tex >> str >> t2.nor
							>> t3.vec >> str >> t3.tex >> str >> t3.nor;
				inds.push_back(t1);
				inds.push_back(t2);
				inds.push_back(t3);
			}
		}
		HRESULT hr;
		hr = gD3dDevice->CreateVertexBuffer(meshtofill->NumofVerts*sizeof(Vertex),0,0,D3DPOOL_MANAGED,&meshtofill->_vb,0);
		hr = gD3dDevice->CreateIndexBuffer( meshtofill->PrimitiveCount*3*sizeof(int),D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_MANAGED,&meshtofill->_ib,0);
		Vertex *Points	= new Vertex[meshtofill->NumofVerts];
		int		*Index	= new int[meshtofill->PrimitiveCount*3];
		unsigned int max = (inds.size());
		for (unsigned int a=0; a<max; a+=3)
		{
			Points[a].position	= pos[inds[a].vec-1];
			Points[a].normal	= nor[inds[a].nor-1];
			Points[a].UVs		= tex[inds[a].tex-1];
			Index[a]=a;
			Points[a+1].position= pos[inds[a+1].vec-1];
			Points[a+1].normal	= nor[inds[a+1].nor-1];
			Points[a+1].UVs		= tex[inds[a+1].tex-1];
			Index[a+1]=a+1;
			Points[a+2].position= pos[inds[a+2].vec-1];
			Points[a+2].normal	= nor[inds[a+2].nor-1];
			Points[a+2].UVs		= tex[inds[a+2].tex-1];
			Index[a+2]=a+2;

			D3DXVECTOR2 uv1 = Points[a+1].UVs - Points[a].UVs;
			D3DXVECTOR2 uv2 = Points[a+2].UVs - Points[a].UVs;
			D3DXVECTOR3 v1	= Points[a+1].position - Points[a].position;
			D3DXVECTOR3 v2	= Points[a+2].position - Points[a].position;
			float		r	= 1.0f/ (uv1.x*uv2.y - uv2.x * uv1.y);
			Points[a].tangent = Points[a+1].tangent = Points[a+2].tangent = D3DXVECTOR3( (uv2.y*v1.x - uv1.y *v2.x) * r, (uv2.y*v1.y - uv1.y *v2.y) * r,  (uv2.y*v1.z - uv1.y *v2.z) * r);
		}
		Vertex *ptr = NULL;

		(*meshtofill->_vb).Lock(0, 0, (void**)&ptr, 0);
		memcpy((void*)ptr, (void*)Points, meshtofill->NumofVerts*sizeof(Vertex));
		(*meshtofill->_vb).Unlock();

		WORD *iptr = NULL;

		(*meshtofill->_ib).Lock(0, 0, (void**)&iptr, 0);
		memcpy((void*)iptr, (void*)Index, meshtofill->PrimitiveCount*3*sizeof(int));
		(*meshtofill->_ib).Unlock();

		delete [] Points;
		delete [] Index;
		infile.close();
	}
}