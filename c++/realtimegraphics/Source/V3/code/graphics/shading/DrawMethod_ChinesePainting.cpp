#include "DrawMethod_ChinesePainting.h"
#include "..\VisualInterface.h"
#include "..\..\input\InputInterface.h"

DrawMethod_ChinesePainting::DrawMethod_ChinesePainting(LPDIRECT3DDEVICE9 device,SceneData *sd) 
{
	d3DDevice=device;
	sceneData=sd;
	SRT		= SRT_Colour;
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\Version2\\ChinesePainting.fx",NULL,NULL,0,NULL,&UberEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	HRESULT hr;
	//////////////////////////////////////////////////////////////////////////
	// G buffer
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &PositionTexture, NULL);
	hr	= PositionTexture->GetSurfaceLevel(0,&PositionSurface);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &NormalTexture, NULL);
	hr	= NormalTexture->GetSurfaceLevel(0,&NormalSurface);
	//
//	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A8R8G8B8),		D3DPOOL_DEFAULT, &ColourTexture, NULL);
//	hr	= ColourTexture->GetSurfaceLevel(0,&ColourSurface);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &ColourTexture, NULL);
	hr	= ColourTexture->GetSurfaceLevel(0,&ColourSurface);
	//
	ScreenQuad.triangle(device);
}

DrawMethod_ChinesePainting::~DrawMethod_ChinesePainting()
{
	PositionTexture->Release();
	PositionSurface->Release();
	NormalTexture->Release();
	NormalSurface->Release();
	ColourTexture->Release();
	ColourSurface->Release();
}

void DrawMethod_ChinesePainting::RenderScene(DM_Output* out)
{
	static bool sr=false;
	int issr = (int)inputinterface::I()->getaction("switch render");
	if (issr && !sr)
	{
		SRT = (SelectRenderTarget)((SRT + 1)%SRT_NumberOfSRTs);
		sr = true;
	}
	else if (!issr)
	{
		sr = false;
	}
	if (out && UberEffect)
	{
		output = out;
		CreateGBuffer();
	}
}

void DrawMethod_ChinesePainting::CreateGBuffer()
{
	d3DDevice->SetDepthStencilSurface(output->Depth);
	d3DDevice->SetRenderTarget(0,PositionSurface);
	d3DDevice->SetRenderTarget(1,NormalSurface);
	d3DDevice->SetRenderTarget(2,ColourSurface);

	if (SRT==SRT_Position)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
	}
	else if (SRT == SRT_Normal)
	{
		d3DDevice->SetRenderTarget(1,output->Render);
	}
	else if (SRT == SRT_Colour)
	{
		d3DDevice->SetRenderTarget(2,output->Render);
	}

	HRESULT hr;
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],UberEffect);
		SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,UberEffect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			for (int modnum=0; modnum < (int)sceneData->Models.size(); modnum++)
			{
				SetWorldMatrix(*sceneData->Models[modnum],UberEffect);
				UberEffect->SetFloatArray			("BaseCol",sceneData->Models[modnum]->BaseCol,3);
				if (sceneData->Models[modnum]->normalMapID!=-1)
				{
					UberEffect->SetTexture				("NormalTexture",sceneData->Textures[sceneData->Models[modnum]->normalMapID]);
				}
				UberEffect->BeginPass(0);
				hr = d3DDevice->SetStreamSource		(0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_vb,0,44 );
				hr = d3DDevice->SetIndices			(sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->NumofVerts,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->PrimitiveCount);
				UberEffect->EndPass();
			}
			UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderTarget(0,output->Render);
	d3DDevice->SetRenderTarget(1,NULL);
	d3DDevice->SetRenderTarget(2,NULL);
	d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET ,D3DCOLOR_XRGB(0,0,0),1.0f,0);
}