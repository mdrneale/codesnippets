#include "DrawMethod_AmbientOcclusion.h"
#include "..\VisualInterface.h"

DrawMethod_AmbientOcclusion::DrawMethod_AmbientOcclusion(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice			= device;
	sceneData			= sd;
	PositionPass		= NULL;
	NormalPass			= NULL;
	LPD3DXBUFFER code	= NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\AmbientOcclusion.fx",NULL,NULL,0,NULL,&AmbientOcclusionEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\blur.fx",NULL,NULL,0,NULL,&BlurEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	ScreenQuad.triangle(d3DDevice);

	HRESULT hr;
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	1, D3DUSAGE_RENDERTARGET, ((_D3DFORMAT)113),	D3DPOOL_DEFAULT, &AOPass, NULL);
	hr	= AOPass->GetSurfaceLevel(0,&AOSurface);

}

void DrawMethod_AmbientOcclusion::RenderScene(DM_Output* out)
{
	if (out && PositionPass && NormalPass)
	{
//		d3DDevice->SetRenderTarget(0,AOSurface);
		d3DDevice->SetRenderTarget(0,out->Render);
		RenderAO();
//		RenderBlur();
	}
}

void DrawMethod_AmbientOcclusion::RenderAO()
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],AmbientOcclusionEffect);
	SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,AmbientOcclusionEffect);
	hr = AmbientOcclusionEffect->SetTexture("PositionTexture",PositionPass);
	hr = AmbientOcclusionEffect->SetTexture("NormalTexture",NormalPass);
	hr = AmbientOcclusionEffect->SetFloat("DistanceOfSample",0.05f);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = AmbientOcclusionEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(AmbientOcclusionEffect->SetTechnique(hTech)))
		{
			hr = AmbientOcclusionEffect->Begin(&Passes,0);
			hr = AmbientOcclusionEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = AmbientOcclusionEffect->EndPass();
			hr = AmbientOcclusionEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
}

void DrawMethod_AmbientOcclusion::RenderBlur()
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	hr = BlurEffect->SetTexture("ImageTexture",AOPass);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = BlurEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(BlurEffect->SetTechnique(hTech)))
		{
			hr = BlurEffect->Begin(&Passes,0);
			hr = BlurEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = BlurEffect->EndPass();
			hr = BlurEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
}
