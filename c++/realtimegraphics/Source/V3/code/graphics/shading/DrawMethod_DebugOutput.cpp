#include "DrawMethod_DebugOutput.h"
#include "..\VisualInterface.h"
DrawMethod_DebugOutput* DrawMethod_DebugOutput::Instance	=	NULL;

DrawMethod_DebugOutput* DrawMethod_DebugOutput::GetDebugger(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	if (!Instance)
	{
		Instance= new DrawMethod_DebugOutput(device,sd);
	}
	return Instance;
}

DrawMethod_DebugOutput::DrawMethod_DebugOutput(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\DebugShader.fx",NULL,NULL,0,NULL,&DebugEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}

	displayTexture	= NULL;
	displaySurface	= NULL;

	ScreenQuad.triangle(d3DDevice);

	HRESULT hr;
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, ((_D3DFORMAT)116),	D3DPOOL_DEFAULT, &displayTexture, NULL);
	hr	= displayTexture->GetSurfaceLevel(0,&displaySurface);
}

void DrawMethod_DebugOutput::RenderScene(DM_Output* out)
{
	if (out)
	{
//		d3DDevice->SetRenderTarget(0,displaySurface);
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(1,0,1),1.0f,0);
		Render();
	}
}

void DrawMethod_DebugOutput::DisplayTexture(D3DXVECTOR2 ScreenPos , D3DXVECTOR2 Scale, LPDIRECT3DTEXTURE9 TextureToDisplay)
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderTarget(0,displaySurface);
	SetWorldMatrix(ScreenPos,Scale,DebugEffect);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		DebugEffect->SetTexture("DisplayTexture",TextureToDisplay);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = DebugEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(DebugEffect->SetTechnique(hTech)))
		{
			DebugEffect->Begin(&Passes,0);
			DebugEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			DebugEffect->EndPass();
		}
		DebugEffect->End();
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_DebugOutput::Render()
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	SetWorldMatrix(D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(1.0f,1.0f),DebugEffect);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		DebugEffect->SetTexture("DisplayTexture",displayTexture);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = DebugEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(DebugEffect->SetTechnique(hTech)))
		{
			DebugEffect->Begin(&Passes,0);
			DebugEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			DebugEffect->EndPass();
		}
		DebugEffect->End();
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
