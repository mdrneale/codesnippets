#include "DrawMephod_BlendOpperations.h"
#include "..\VisualInterface.h"

DrawMephod_BlendOpperations::DrawMephod_BlendOpperations(LPDIRECT3DDEVICE9 device,SceneData *sd) 
{
	d3DDevice=device;
	sceneData=sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\blend.fx",NULL,NULL,0,NULL,&BlendEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	ScreenQuad.triangle(d3DDevice);
	Pass1		= NULL;
	Pass2		= NULL;
	Opp = 0;
}

void DrawMephod_BlendOpperations::RenderScene(DM_Output* out)
{
	if (out && Pass1 && Pass2)
	{
		BlendEffect->SetTexture	("Texture1"	,Pass1);
		BlendEffect->SetTexture	("Texture2"	,Pass2);
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		Blend();	
	}
}

void DrawMephod_BlendOpperations::Blend()
{
	HRESULT hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech;
		UINT Passes;
		hTech = BlendEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(BlendEffect->SetTechnique(hTech)))
		{
			BlendEffect->Begin(&Passes,0);
			BlendEffect->BeginPass(Opp);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			BlendEffect->EndPass();
			BlendEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
}

D3DXHANDLE DrawMephod_BlendOpperations::GetBlendMode()
{
	switch (Opp)
	{
	case 0:
		return	BlendEffect->GetTechniqueByName("add");		
	}
	return	BlendEffect->GetTechniqueByName("add");
}