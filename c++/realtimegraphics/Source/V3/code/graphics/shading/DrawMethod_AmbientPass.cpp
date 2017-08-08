#include "DrawMethod_AmbientPass.h"
#include "..\VisualInterface.h"

DrawMethod_AmbientPass::DrawMethod_AmbientPass(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\ambient.fx",NULL,NULL,0,NULL,&AmbientColourEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}

	ScreenQuad.triangle(d3DDevice);
	PositionPass	= NULL;
	NormalPass		= NULL;
	ColourPass		= NULL;
}

void DrawMethod_AmbientPass::RenderScene(DM_Output* out)
{
	if (out && PositionPass && NormalPass && ColourPass && AmbientColourEffect)
	{
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
//		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		Render();
	}
}

void DrawMethod_AmbientPass::Render()
{

	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	HRESULT hr;
	hr = AmbientColourEffect->SetTexture("PositionTexture",PositionPass);
	hr = AmbientColourEffect->SetTexture("NormalTexture",NormalPass);
	hr = AmbientColourEffect->SetTexture("ColourTexture",ColourPass);
	hr = AmbientColourEffect->SetFloatArray("AmbientLight",&sceneData->AmbientColour.R,3);

	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = AmbientColourEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(AmbientColourEffect->SetTechnique(hTech)))
		{
			SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],AmbientColourEffect);
			SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,AmbientColourEffect);
			hr = AmbientColourEffect->Begin(&Passes,0);
			hr = AmbientColourEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = AmbientColourEffect->EndPass();
			hr = AmbientColourEffect->End();
		}
		hr = d3DDevice->EndScene();
	}

	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}