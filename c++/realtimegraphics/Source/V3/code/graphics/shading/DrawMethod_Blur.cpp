#include "DrawMethod_Blur.h"
#include "..\VisualInterface.h"

DrawMethod_Blur::DrawMethod_Blur(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\Blur.fx",NULL,NULL,0,NULL,&BlurEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	TempBlurSurface	= NULL;
	BlurTexture		= NULL;
	ScreenQuad.triangle(device);
}

void DrawMethod_Blur::RenderScene(DM_Output* out)
{
	if (out && BlurEffect && TempBlurSurface && BlurTexture)
	{
		d3DDevice->SetRenderTarget(0,TempBlurSurface);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		Render(out);
	}
}

void DrawMethod_Blur::Render(DM_Output* out)
{
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	HRESULT hr;
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = BlurEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(BlurEffect->SetTechnique(hTech)))
		{
			BlurEffect->Begin(&Passes,0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			BlurEffect->SetTexture("BlurTexture",BlurTexture);
			BlurEffect->BeginPass(0);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			BlurEffect->EndPass();
			BlurEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderTarget(0,out->Render);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = BlurEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(BlurEffect->SetTechnique(hTech)))
		{
			BlurEffect->Begin(&Passes,0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			BlurEffect->SetTexture("BlurTexture",TempBlurTexture);
			BlurEffect->BeginPass(1);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			BlurEffect->EndPass();
			BlurEffect->End();
		}
		hr = d3DDevice->EndScene();
	}

	d3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
