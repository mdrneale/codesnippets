#include "DrawMethod_TvStatic.h"
#include "..\VisualInterface.h"
#include "..\..\input\InputInterface.h"

DrawMethod_TvStatic::DrawMethod_TvStatic(LPDIRECT3DDEVICE9 device,SceneData *sd) 
{
	d3DDevice=device;
	sceneData=sd;
	
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\tvstatic.fx",NULL,NULL,0,NULL,&TVStaticEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	ScreenQuad.triangle(d3DDevice);
	tvStaticTextureID		= VisualInterface::I()->LoadTexture("textures\\tv.tga");
	preprocessRender		= NULL;
	TVStaticEffect->SetTexture("TvStatic",sceneData->Textures[tvStaticTextureID]);
	T.Duration		= 0.05f;
	currentFrame	= 0;
	tvStaticWeight	= 0.025f;
}

void DrawMethod_TvStatic::RenderScene(DM_Output* out)
{
	if (out && preprocessRender)
	{
		TVStaticEffect->SetInt		("Time"			,currentFrame);
		TVStaticEffect->SetFloat	("Weight"		,tvStaticWeight);
		TVStaticEffect->SetTexture	("SceneTexture"	,preprocessRender);
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		TvStatic();	
	}
}

void DrawMethod_TvStatic::TvStatic()
{
	if (T.Check())
	{
		currentFrame=(currentFrame+1)%15;
	}
	HRESULT hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(1,1,1),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech;
		UINT Passes;
		hTech = TVStaticEffect->GetTechniqueByName("TvStaticRender");
		if (SUCCEEDED(TVStaticEffect->SetTechnique(hTech)))
		{
			TVStaticEffect->Begin(&Passes,0);
			TVStaticEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			TVStaticEffect->EndPass();
			TVStaticEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
}