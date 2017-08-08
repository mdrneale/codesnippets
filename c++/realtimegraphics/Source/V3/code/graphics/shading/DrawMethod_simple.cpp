#include "DrawMethod_simple.h"
#include "..\VisualInterface.h"

DrawMethod_simple::DrawMethod_simple(LPDIRECT3DDEVICE9 device,SceneData *sd) 
{
	d3DDevice=device;
	sceneData=sd;
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\Default9.fx",NULL,NULL,0,NULL,&simpleEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
}

void DrawMethod_simple::RenderScene()
{
	if (SUCCEEDED(d3DDevice->BeginScene()))	//gets the D3D device ready to draw the screen
	{
		D3DXHANDLE hTech;
		UINT Passes;
		HRESULT hr;
		simpleEffect->FindNextValidTechnique(NULL,&hTech);
		if (SUCCEEDED(simpleEffect->SetTechnique(hTech)))
		{
			simpleEffect->Begin(&Passes,0);
			for (UINT b=0; b < Passes; b++)
			{
				simpleEffect->BeginPass(b);
				hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[0]->_vb,0,44 );
				hr = d3DDevice->SetIndices(sceneData->Meshes[0]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[0]->NumofVerts,0,sceneData->Meshes[0]->PrimitiveCount);
				simpleEffect->EndPass();
			}
			simpleEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
}